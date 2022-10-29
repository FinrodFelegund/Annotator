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
    //_manager = std::make_shared<Manager>();
    setScene(new QGraphicsScene());
    setBackgroundBrush(QBrush(QColor(252, 252, 252)));
    scene()->setBackgroundBrush(QBrush(QColor(252, 252, 252)));
    _tileSize = 1024;
    _lastLevel = 0;
    _currentScaleFactor = 0;
    _sceneScale = 0;


    setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorViewCenter);
    setDragMode(QGraphicsView::DragMode::NoDrag);
    setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
    setViewportUpdateMode(ViewportUpdateMode::FullViewportUpdate);
    setInteractive(false);
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
    _manager = std::make_shared<Manager>(this, _reader, _tileSize);
    _lastLevel = reader->getNumberOfLevels() - 1;
    _sceneScale = _reader->getLevelDownSample(_lastLevel);


    std::pair<int, int> dimensions0 = _reader->getLevel0Dimensions();
    std::pair<int, int> topDimension = _reader->getLevelDimensions(_reader->getNumberOfLevels() - 1);
    qDebug() << "Dimension 0: " << dimensions0.first << " " << dimensions0.second;
    qDebug() << "Top Dimension: " << topDimension.first << " " << topDimension.second;
    setSceneRect(0, 0, dimensions0.first, dimensions0.second);
    fitInView(QRectF(0, 0, dimensions0.first, dimensions0.second), Qt::KeepAspectRatio);

    std::vector<Worker*> worker = _manager->getThreads();
    for(int i = 0; i < worker.size(); i++)
    {
        connect(worker[i], &Worker::finished, this, &AnnotatorViewer::loadTileInScene);
    }

    _reader->printLevelDownsample();

    loadAllTilesForLevel(_reader->getNumberOfLevels() - 1);

}

void AnnotatorViewer::loadAllTilesForLevel(int level)
{
    unsigned int numberOfLevels = _reader->getNumberOfLevels();


    std::pair<int, int> dimensionsTop = _reader->getLevelDimensions(numberOfLevels - 1);
    std::pair<int, int> dimensions0 = _reader->getLevel0Dimensions();
    int tilesX = qCeil((qreal)dimensionsTop.first / (qreal)_tileSize);
    int tilesY = qCeil((qreal)dimensionsTop.second / (qreal)_tileSize);

    //qDebug() << "Number of Tiles: " << tilesX << " " << tilesY;

    int scale = qCeil((qreal)dimensions0.first / (qreal)dimensionsTop.first);

    int width, height = 0;
    for(int i = 0; i < tilesX; i++)
    {
        for(int j = 0; j < tilesY; j++)
        {
            if(i == tilesX - 1)
            {
                width = (dimensionsTop.first - i * _tileSize) - 1;
            } else
            {
                width = _tileSize;
            }

            if(j == tilesY - 1)
            {
                height = (dimensionsTop.second - j * _tileSize) - 1;
            } else
            {
                height = _tileSize;
            }

            int tmpX = i * _tileSize * scale;
            int tmpY = j *_tileSize * scale;
            //qDebug() << "Tile Coordinate in set tiles: " << tmpX << " " << tmpY << " " << width << " " << height;
            _manager->addJob(i * _tileSize * scale, j * _tileSize * scale, numberOfLevels - 1, width, height);
        }
    }
    //qDebug() << "Number of tiles: " << tilesX << " " << tilesY;
}

void AnnotatorViewer::loadTilesForFieldOfView(QRect view, int level)
{

    int tilesX = qCeil(((qreal)view.width()) / ((qreal)_tileSize));
    int tilesY = qCeil(((qreal)view.height()) / ((qreal)_tileSize));

    //qDebug() << "Number of Tiles: " << tilesX << " " << tilesY;

    int width, height = 0;

    std::pair<int, int> dims0 = _reader->getLevel0Dimensions();
    std::pair<int, int> dimsLevel = _reader->getLevelDimensions(level);
    int scale = qCeil(qreal(dims0.first) / qreal(dimsLevel.first));

    for(int i = 0; i < tilesX; i++)
    {
        for(int j = 0; j < tilesY; j++)
        {
            if(i == tilesX - 1)
            {
                width = (view.width() - i * _tileSize) - 1;
            } else
            {
                width = _tileSize;
            }

            if(j == tilesY - 1)
            {
                height = (view.height() - j * _tileSize) - 1;
            } else
            {
                height = _tileSize;
            }

            _manager->addJob((view.x() + i * _tileSize) * scale, (view.y() + j * _tileSize) * scale, level, width, height);
        }

    }

}

void AnnotatorViewer::loadTileInScene(unsigned int *buf, int x, int y, int width, int height, int level)
{


    std::pair<int, int> level0 = _reader->getLevel0Dimensions();
    std::pair<int, int> currentLevel = _reader->getLevelDimensions(level);
    qreal scaleFactor = (qreal)level0.first / (qreal)currentLevel.first;

    QImage *img = nullptr;
    QGraphicsPixmapItem *item = nullptr;
    img = new QImage((unsigned char*)buf, width, height,  QImage::Format_ARGB32);
    //qDebug() << "x: " << x << " y: " << y << " width: " << width << " height: " << height;
    item = scene()->addPixmap(QPixmap::fromImage(*img));
    delete img;
    item->setScale(scaleFactor);
    item->setPos(x, y);

}

void AnnotatorViewer::close()
{
    _reader = nullptr;
    if(_manager)
    {
        _manager->shutdown();
        _manager->clearJobs();
        _manager->deleteLater();
        _manager = nullptr;
    }
    setEnabled(false);
    scene()->clear();
    _currentScaleFactor = 0;
    _lastLevel = 0;
    _sceneScale = 0;
}

void AnnotatorViewer::wheelEvent(QWheelEvent *event)
{
    std::pair<int, int> dims0 = _reader->getLevel0Dimensions();
    QRectF FOV = this->mapToScene(this->rect()).boundingRect();
    //qDebug() << "x: " << FOV.x() << " y: " << FOV.y() << " width: " << FOV.width() << " height: " << FOV.height();

    std::pair<int, int> topLevelDimension = _reader->getLevelDimensions(_reader->getNumberOfLevels() - 1);
    std::pair<int, int> level0Dimension = _reader->getLevel0Dimensions();
    qreal scaleX = qreal(dims0.first) / qreal(FOV.width());

    float angle = event->angleDelta().y();
    qreal numDegrees = qPow(1.2, angle / 240.0);

    int topLevelDownsample = _reader->getLevelDownSample(_reader->getNumberOfLevels() - 1);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    if(angle > 1 && topLevelDownsample >= scaleX)
    {
        if(qreal(FOV.width() / numDegrees < topLevelDimension.first)) // scale only till width of toplevel image is reached
        {
            numDegrees = qreal(FOV.width()) / qreal(topLevelDimension.first);
        }

        scale(numDegrees, numDegrees);

    } else if((angle < 1) && (1 <= scaleX))
    {
        if(qreal(FOV.width()) / numDegrees > level0Dimension.first)
        {
            numDegrees = qreal(FOV.width()) / qreal(level0Dimension.first); // scale only till width of bottomlevel is reached
        }

        scale(numDegrees, numDegrees);
    }

    int projectedLevel = _reader->getLevelForGivenDownSample(scaleX);
    if(projectedLevel != _lastLevel )
    {
        _lastLevel = projectedLevel;
        float sceneScale =   _reader->getLevelDownSample(projectedLevel);
        QRect intFOV = QRect(FOV.left() / sceneScale, FOV.top() / sceneScale, FOV.width() / sceneScale, FOV.height() / sceneScale);
        //this->scene()->clear();
        //qDebug() << "FOV: " << intFOV.top() << " " << intFOV.left() << " " << intFOV.width() << " " << intFOV.height();
        //qDebug() << " ";
        loadTilesForFieldOfView(intFOV, _lastLevel);
    }
}

void AnnotatorViewer::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "Mouse moving";
}

void AnnotatorViewer::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "Mouse pressed";
}

void AnnotatorViewer::mouseReleaseEvent(QMouseEvent *event)
{
   //qDebug() << "Mouse released";
}