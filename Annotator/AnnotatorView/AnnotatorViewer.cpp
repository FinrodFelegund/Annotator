//
// Created by Daniel Pietsch on 08.08.22.
//

#include "AnnotatorViewer.h"
#include "../Threads/Worker.h"
#include <QLabel>
#include <QImage>
#include <QWheelEvent>
#include "../Data Structures/Tile.h"


AnnotatorViewer::AnnotatorViewer(QObject *parent)
{

    setScene(new QGraphicsScene());
    setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    scene()->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    _tileSize = 1024;
    _currentLevel = 0;
    _currentSceneScale = 0;
    _levelZeroDimensions.first = _levelZeroDimensions.second = 0;
    _clicked = false;
    _currentPos.setX(0);
    _currentPos.setY(0);
    _map = nullptr;
    _longestSide = 0;

    setDragMode(QGraphicsView::DragMode::NoDrag);
    setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
    setInteractive(false);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEnabled(false);

    setRenderHints(QPainter::Antialiasing);

    setUI();

}

AnnotatorViewer::~AnnotatorViewer() noexcept
{
    close();
    delete scene();
}

void AnnotatorViewer::initialize(std::shared_ptr<ImageReader> reader)
{

    close();
    setEnabled(true);
    _reader = reader;
    _currentLevel = reader->getNumberOfLevels() - 1;
    _currentSceneScale = _reader->getLevelDownSample(_currentLevel);
    _levelZeroDimensions = _reader->getLevel0Dimensions();
    _longestSide = _levelZeroDimensions.first > _levelZeroDimensions.second ? _levelZeroDimensions.first : _levelZeroDimensions.second;
    _levelTopDimensions = _reader->getLevelDimensions(_currentLevel);
    unsigned char *buf = _reader->readDataFromImage(0, 0, _levelTopDimensions.first, _levelTopDimensions.second, _reader->getNumberOfLevels()-1);
    QImage img(buf, _levelTopDimensions.first, _levelTopDimensions.second, 3 * _levelTopDimensions.first, QImage::Format_RGB888);
    //_map = new MiniMap(QPixmap::fromImage(img), _reader->getScaleFactor(_reader->getNumberOfLevels() - 1), this);
    _map->setPixmap(QPixmap::fromImage(img));
    _map->setDownSample(_reader->getScaleFactor(_reader->getNumberOfLevels() - 1));
    if(this->layout())
    {
        delete this->layout();
    }
    QVBoxLayout * Hlayout = new QVBoxLayout(this);
    Hlayout->setAlignment(Qt::AlignTop);
    Hlayout->setContentsMargins(0, 15, 15, 0);
    if(_map)
    {
        Hlayout->addWidget(_map, 4, Qt::AlignRight);
    }

    _map->setHidden(false);
    _map->initalize();

    setSceneRect(0, 0, _levelZeroDimensions.first, _levelZeroDimensions.second);
    fitInView(QRectF(0, 0, _longestSide, _longestSide), Qt::KeepAspectRatio);

}

QRectF AnnotatorViewer::getSceneRect()
{
    return this->sceneRect();
}

int AnnotatorViewer::getCurrentLevel()
{
    return _currentLevel;
}

qreal AnnotatorViewer::getCurrentSceneScale()
{
    return _currentSceneScale;
}

int AnnotatorViewer::getTileSize()
{
    return _tileSize;
}

void AnnotatorViewer::loadTileInScene(Tile tile)
{
    if(!_reader)
    {
        //I think if still some signals are stored in this connection, this code will be executed regardless of current state (eg _reader is not valid anymore)
        return;
    }

    int bytesPerLine = 3;
    QImage img((unsigned char*)tile.getBuf(), tile.getWidth(), tile.getHeight(), bytesPerLine* tile.getWidth(), QImage::Format_RGB888);
    GraphicsItem *newItem = new GraphicsItem(QPixmap(QPixmap::fromImage(img)), _reader->getScaleFactor(tile.getLevel()), tile.getLevel(), tile.getX(), tile.getY());
    if(tile.getLevel() != _currentLevel)
    {
        newItem->setOpacity(0);
    } else
    {
        newItem->setOpacity(1.);
    }

    scene()->addItem(newItem);

    newItem->setPos(tile.getX(), tile.getY());
    newItem->setScale(_reader->getLevelDownSample(tile.getLevel()));
    //newItem->setZValue(tile.getLevel());


    emit itemLoaded(newItem);
}

void AnnotatorViewer::close()
{
    //_reader belongs to the Controller Object
    if(_reader)
    {
        _reader = nullptr;
    }

    if(_map)
    {
        _map->setHidden(true);
        _map->reset();
    }


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
    QRectF rect = this->mapToScene(this->rect()).boundingRect(); //currently visible rect of scene in view
    qreal currentlyScaledValue = _levelZeroDimensions.first > _levelZeroDimensions.second ? qreal(_levelZeroDimensions.first) / qreal(rect.width()) : qreal(_levelZeroDimensions.second) / qreal(rect.height());
    int levelTopDownsample = _reader->getLevelDownSample(_reader->getNumberOfLevels() - 1);
    auto point = event->position().toPoint();
    auto scenePos = this->mapToScene(point);
    //qDebug() << "rect: " << rect.x() << " " << rect.y() << " " << rect.width() << " " << rect.height();



    //compute scaled angle
    float angle = event->angleDelta().y();
    qreal numDegrees = std::pow(1.2, angle / 240.);

    //scale
    if(angle > 1 && levelTopDownsample * 2.5 >= currentlyScaledValue) //angle > 1 means scaling down; lTD >= cSV means scaling down till view is as wide as top Level width
    {
        if(qreal(rect.width()) * 2.5 / numDegrees < _levelTopDimensions.first)
        {
            numDegrees = qreal(rect.width()) * 2.5 / qreal(_levelTopDimensions.first); //scale only till width of top level is reached
        }
        scale(numDegrees, numDegrees);
        const QPointF p1mouse = mapFromScene(scenePos);
        const QPointF move = p1mouse - event->position(); // The move
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());

    } else if(angle < 1 && currentlyScaledValue <= (2.5 * _longestSide))
    {
        if(qreal(rect.width()) / numDegrees > (2.5 * _longestSide))
        {
            numDegrees = qreal(rect.width()) / qreal(2.5 * _longestSide);
        }
        scale(numDegrees, numDegrees);
        const QPointF p1mouse = mapFromScene(scenePos);
        const QPointF move = p1mouse - event->position(); // The move
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
    }

    rect = this->mapToScene(this->rect()).boundingRect();
    emit fieldOfViewForMinimapChanged(rect);

    //find out on which level we are after scaling
    int projectedLevel = _reader->getLevelForGivenDownSample(currentlyScaledValue);
    if(projectedLevel != _currentLevel)
    {
        _currentLevel = projectedLevel;
        _currentSceneScale = currentlyScaledValue;
        std::pair<int, int> newLevel = _reader->getLevelDimensions(_currentLevel);
        emit levelChanged(QRectF(0, 0, newLevel.first, newLevel.second));
        emit fieldOfViewChanged(rect);
    }

}

void AnnotatorViewer::resizeEvent(QResizeEvent *event)
{
    if(!isEnabled() || !_reader)
    {
        return;
    }

    QRect r(QPoint(0, 0), event->size());
    QRectF rect = this->mapToScene(r).boundingRect();

    QGraphicsView::resizeEvent(event);
    keepViewInCheck(rect);
    QRectF after = this->mapToScene(this->rect()).boundingRect();


    emit fieldOfViewForMinimapChanged(after);
    emit fieldOfViewChanged(rect);

}



void AnnotatorViewer::mousePressEvent(QMouseEvent *event)
{
    _clicked = true;
    _currentPos = event->pos();
    _currentScene = this->mapToScene(this->rect()).boundingRect();
}

void AnnotatorViewer::mouseMoveEvent(QMouseEvent *event)
{
    if(_clicked)
    {
        QPoint delta = _currentPos.toPoint() - event->pos();
        _currentPos = event->pos();
        horizontalScrollBar()->setValue(delta.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(delta.y() + verticalScrollBar()->value());

        QRectF rect = mapToScene(this->rect()).boundingRect();

        //find out if the tiles we pan to are covering or if we have to reload the fieldofview
        emit fieldOfViewChanged(rect);
        emit fieldOfViewForMinimapChanged(rect);


    }
}

void AnnotatorViewer::mouseReleaseEvent(QMouseEvent *event)
{
   _clicked = false;
}

void AnnotatorViewer::setMinimap()
{
    _map = new MiniMap(this);
    connect(this, &AnnotatorViewer::fieldOfViewForMinimapChanged, _map, &MiniMap::updateFieldOfView);
    connect(_map, &MiniMap::mapClicked, this, &AnnotatorViewer::centerOnMinimap);
    connect(_map, &MiniMap::mapDragged, this, &AnnotatorViewer::dragInMiniMap);

}

void AnnotatorViewer::centerOnMinimap(QPointF point)
{
    centerOn(point);
    QRectF rect = this->mapToScene(this->rect()).boundingRect();
    emit fieldOfViewChanged(rect);
    emit fieldOfViewForMinimapChanged(rect);
}



void AnnotatorViewer::setUI()
{
    //for now just setup Minimap and connect its signals and slots
    setMinimap();
}

void AnnotatorViewer::keepViewInCheck(QRectF rect)
{
    if(!_reader)
    {
        return;
    }
    //prevent the resized views' width to be wider than allowed with of 2.5 * _levelzerodims.first
    {
        if(rect.width() > 2.5 * _longestSide)
        {
            float numDegrees = rect.width() / (2.5 * _longestSide);
            scale(numDegrees, numDegrees);
        }
    }


    rect = this->mapToScene(this->rect()).boundingRect();
    //if(_longestSide == _levelZeroDimensions.second)
    {
        if(rect.height() > (_longestSide * 2.5))
        {
            float numDegrees = rect.height() / (2.5 * _longestSide);
            scale(numDegrees, numDegrees);
        }
    }

}

void AnnotatorViewer::dragInMiniMap(QPointF point)
{
    centerOn(point);
    QRectF rect = this->mapToScene(this->rect()).boundingRect();
    emit fieldOfViewForMinimapChanged(rect);
    emit fieldOfViewChanged(rect);
}

void AnnotatorViewer::enable(bool val)
{
    this->setEnabled(val);
}


