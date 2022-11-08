//
// Created by Daniel Pietsch on 08.08.22.
//

#include "AnnotatorViewer.h"
#include "../Threads/Worker.h"
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QImage>
#include <QWheelEvent>

AnnotatorViewer::AnnotatorViewer(QObject *parent)
{

    setScene(new QGraphicsScene());
    setBackgroundBrush(QBrush(QColor(252, 252, 252)));
    scene()->setBackgroundBrush(QBrush(QColor(252, 252, 252)));
    _tileSize = 1024;
    _currentLevel = 0;
    _currentSceneScale = 0;
    _levelZeroDimensions.first = _levelZeroDimensions.second = 0;
    _clicked = false;
    _currentPos.setX(0);
    _currentPos.setY(0);


    setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorViewCenter);
    setDragMode(QGraphicsView::DragMode::NoDrag);
    setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
    setViewportUpdateMode(ViewportUpdateMode::FullViewportUpdate);
    setInteractive(false);
    horizontalScrollBar()->hide();
    verticalScrollBar()->hide();
    setEnabled(false);

    setRenderHints(QPainter::Antialiasing);

}

AnnotatorViewer::~AnnotatorViewer() noexcept
{
    close();
}

void AnnotatorViewer::initialize(std::shared_ptr<WholeSlideImageReader> reader)
{

    close();
    setEnabled(true);
    _reader = reader;
    //_manager = std::make_shared<Manager>(this, _reader, _tileSize);
    _currentLevel = reader->getNumberOfLevels() - 1;
    _currentSceneScale = _reader->getLevelDownSample(_currentLevel);
    _levelZeroDimensions = _reader->getLevel0Dimensions();
    _levelTopDimensions = _reader->getLevelDimensions(_currentLevel);

    setSceneRect(0, 0, _levelZeroDimensions.first, _levelZeroDimensions.second);
    fitInView(QRectF(0, 0, _levelZeroDimensions.first, _levelZeroDimensions.second), Qt::KeepAspectRatioByExpanding);

    //this->setRenderHints(QPainter::Antialiasing);
    //setCacheMode(QGraphicsView::CacheBackground);
    //setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
}

QRectF AnnotatorViewer::getSceneRect()
{
    return this->sceneRect();
}

int AnnotatorViewer::getCurrentLevel()
{
    return _currentLevel;
}

int AnnotatorViewer::getCurrentSceneScale()
{
    return _currentSceneScale;
}

int AnnotatorViewer::getTileSize()
{
    return _tileSize;
}

void AnnotatorViewer::loadTileInScene(unsigned char *buf, int x, int y, int width, int height, int level)
{

    std::pair<int, int> currentLevel = _reader->getLevelDimensions(level);
    qreal scaleFactor = (qreal)_levelZeroDimensions.first / (qreal)currentLevel.first;

    qDebug() << "Level: " << level << " X: " << x << " Y: " << y << " Buf: " << buf << " Scalefactor: " << scaleFactor << " Width: " << width << " Height: " << height;

    //if(level == 2)

        QImage *img = nullptr;
        QGraphicsPixmapItem *item = nullptr;
        img = new QImage((unsigned char*)buf, width, height,  QImage::Format_RGB888);
        item = scene()->addPixmap(QPixmap::fromImage(*img));
        delete img;
        item->setScale(scaleFactor);
        item->setPos(x, y);
}

void AnnotatorViewer::close()
{
    //_reader belongs to the Controller Object
    _reader = nullptr;
    setEnabled(false);
    scene()->clear();
    _currentLevel = 0;
    _currentSceneScale = 0;
    _levelZeroDimensions.first = _levelZeroDimensions.second = 0;
    _levelTopDimensions.first = _levelTopDimensions.second = 0;
    _clicked = false;
    _currentPos.setX(0);
    _currentPos.setY(0);
}

void AnnotatorViewer::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //declarations
    QRect rect = this->mapToScene(this->rect()).boundingRect().toRect(); //currently visible rect of scene in view
    qreal currentlyScaledValue = qreal(_levelZeroDimensions.first) / qreal(rect.width());
    int levelTopDownsample = _reader->getLevelDownSample(_reader->getNumberOfLevels() - 1);

    auto point = event->position().toPoint();
    auto scenePos = this->mapToScene(point);

    //compute scaled angle
    float angle = event->angleDelta().y();
    qreal numDegrees = std::pow(1.2, angle / 240.);

    //scale
    if(angle > 1 && levelTopDownsample >= currentlyScaledValue) //angle > 1 means scaling down; lTD >= cSV means scaling down till view is as wide as top Level width
    {
        if(qreal(rect.width()) / numDegrees < _levelTopDimensions.first)
        {
            numDegrees = qreal(rect.width()) / qreal(_levelTopDimensions.first); //scale only till width of top level is reached
        }
        scale(numDegrees, numDegrees);
        const QPointF p1mouse = mapFromScene(scenePos);
        const QPointF move = p1mouse - event->position(); // The move
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());

    } else if(angle < 1 && 1. <= _levelZeroDimensions.first)
    {
        if(qreal(rect.width()) / numDegrees > _levelZeroDimensions.first)
        {
            numDegrees = qreal(rect.width()) / qreal(_levelZeroDimensions.first);
        }
        scale(numDegrees, numDegrees);
        const QPointF p1mouse = mapFromScene(scenePos);
        const QPointF move = p1mouse - event->position(); // The move
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
    }

    //find out on which level we are after scaling
    int projectedLevel = _reader->getLevelForGivenDownSample(currentlyScaledValue);
    if(projectedLevel != _currentLevel)
    {
        _currentLevel = projectedLevel;
        _currentSceneScale = _reader->getLevelDownSample(_currentLevel);
        emit fieldOfViewChanged(rect);
    }

}

void AnnotatorViewer::resizeEvent(QResizeEvent *event)
{
    QRect rect = this->mapToScene(this->rect()).boundingRect().toRect();

    if(rect.width() >= _levelZeroDimensions.first || rect.height() >= _levelZeroDimensions.second)
    {
        fitInView(0, 0, _levelZeroDimensions.first, _levelZeroDimensions.second);
    }
    QGraphicsView::resizeEvent(event);
}



void AnnotatorViewer::mousePressEvent(QMouseEvent *event)
{
    _clicked = true;
    _currentPos = event->pos();
}

void AnnotatorViewer::mouseMoveEvent(QMouseEvent *event)
{
    if(_clicked)
    {
        QPoint delta = _currentPos.toPoint() - event->pos();
        _currentPos = event->pos();
        horizontalScrollBar()->setValue(delta.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(delta.y() + verticalScrollBar()->value());
    }
}

void AnnotatorViewer::mouseReleaseEvent(QMouseEvent *event)
{
   _clicked = false;
}