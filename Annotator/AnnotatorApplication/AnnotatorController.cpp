//
// Created by Daniel Pietsch on 04.11.22.
//

#include "AnnotatorController.h"
#include <QScreen>
#include "../Data Structures/Tiler.h"
#include "../Data Structures/TileJob.h"
#include "../Threads/Worker.h"
#include <QStatusBar>


LevelManager::LevelManager()
{
    _currentLevelRect = QRectF();
    _currentFieldOfView = QRectF();
    _tileSize = 1024;
}

LevelManager::~LevelManager()
{

}

void LevelManager::setCurrentLevel(QRectF levelRect)
{
    _currentLevelRect = levelRect;
    _currentFieldOfView = QRectF();
}

void LevelManager::setTileSize(int tileSize)
{
    _tileSize = tileSize;
}

QRectF LevelManager::toLevelRect(QRectF fieldOfView)
{
    //get the next smaller (x,y) floating above rect
    int x = 0, y = 0;
    x = (int)fieldOfView.x() / _tileSize;
    y = (int)fieldOfView.y() / _tileSize;
    x = x * _tileSize;
    y = y * _tileSize;


    //get the width and height offset by tilesize
    int width = 0, height = 0;
    width = (int)fieldOfView.bottomRight().x() / _tileSize;
    height = (int)fieldOfView.bottomRight().y() / _tileSize;
    width = (width * _tileSize) + _tileSize;
    height = (height * _tileSize) + _tileSize;
    if(width > _currentLevelRect.width())
    {
        width = _currentLevelRect.width();
    }

    if(height > _currentLevelRect.height())
    {
        height = _currentLevelRect.height();
    }

    return QRectF(x, y, width - x, height - y);
}

bool LevelManager::isCurrentFieldOfView(QRectF rect)
{
    bool retVal = false;
    if(rect == _currentFieldOfView )
    {
        retVal = true;
    }
    _currentFieldOfView = rect;
    return retVal;
}



AnnotatorController::AnnotatorController(QObject *parent) : QObject(parent)
{
    _window = nullptr;
    _reader = std::make_shared<WholeSlideImageReader>();
    _manager = std::make_shared<Manager>(this);
    _view = std::make_shared<AnnotatorViewer>();
    _levelManager = std::make_shared<LevelManager>();
    _cache = std::make_shared<ItemCache>();

}

AnnotatorController::~AnnotatorController() noexcept
{
    shutDown();
}

void AnnotatorController::shutDown()
{
    if(_window)
    {
        _window->deleteLater();
        _window = nullptr;
    }

    if(_reader)
    {
        _reader = nullptr;
    }

    if(_view)
    {
        _view->deleteLater();
        _view = nullptr;
    }

    if(_levelManager)
    {
        _levelManager = nullptr;
    }

    if(_cache)
    {
        _cache->clear();
        _cache = nullptr;
    }


}

void AnnotatorController::setWindow(std::shared_ptr<AnnotatorMainWindow> window)
{
    if(!window)
    {
        return;
    }

    _window = window;
    if(_view)
    {
        _window->setView(_view);
    }

    connect(_window.get(), &AnnotatorMainWindow::initializeImage, this, &AnnotatorController::initializeImage);
}

void AnnotatorController::initializeImage(std::string fileName)
{

    _reader->initializeImage(fileName);

    if(!_reader->isValid())
    {
        qDebug() << "AnnotatorController: reader is not valid";
        return;
    }
    _manager->shutdown();
    _manager->startWorkers();
    _manager->setImage(_reader);
    if(_view == nullptr)
    {
        _view = _window->getView();
    }


    if(!_view)
    {
        qDebug() << "AnnotatorController: view is not valid";
        return;
    }


    _cache->setScene(_view->scene());
    _cache->setTopLevel(_reader->getNumberOfLevels() - 1);
    _view->initialize(_reader);
    auto dims = _reader->getLevelDimensions(_reader->getNumberOfLevels() - 1);

    std::vector<Worker*> worker = _manager->getThreads();
    for(int i = 0; i < worker.size(); i++)
    {
        connect(worker[i], &Worker::finished, _view.get(), &AnnotatorViewer::loadTileInScene, Qt::QueuedConnection);
    }

    connect(_view.get(), &AnnotatorViewer::itemLoaded, this, &AnnotatorController::itemLoaded);
    connect(_view.get(), &AnnotatorViewer::fieldOfViewChanged, this, &AnnotatorController::fieldOfViewChanged);
    connect(_view.get(), &AnnotatorViewer::levelChanged, this, &AnnotatorController::levelChanged);
    connectActions();


    int level = _view->getCurrentLevel();
    int viewSceneScale = _reader->getLevelDownSample(level);
    int tileSize = _view->getTileSize();
    QRectF sceneRect = _view->getSceneRect();

    sceneRect.setWidth(((int)(sceneRect.width() / viewSceneScale)));
    sceneRect.setHeight(((int)(sceneRect.height() / viewSceneScale)));
    Tiler tiler(sceneRect, tileSize);
    tiler.exec();
    auto tiles = tiler.getTilingResult();
    _manager->setCurrentLevel(level);
    for(int i = 0; i < tiles.size(); i++)
    {
        _manager->addJob(tiles[i].getX() * viewSceneScale, tiles[i].getY() * viewSceneScale, level, tiles[i].getWidth(), tiles[i].getHeight(), JobType::ViewJob);

    }

}

void AnnotatorController::fieldOfViewChanged(QRectF rect)
{
    //qDebug() << "In Viewer Width: " << rect.width() << " Height: " << rect.height();
    int tileSize = _view->getTileSize();
    int level = _view->getCurrentLevel();
    _manager->setCurrentLevel(level);
    float viewSceneScale = _reader->getLevelDownSample(level);
    rect.setX((int)(rect.x() / viewSceneScale));
    rect.setY((int)(rect.y() / viewSceneScale));
    rect.setWidth((int)(rect.width() / viewSceneScale));
    rect.setHeight((int)(rect.height() / viewSceneScale));

    rect = _levelManager->toLevelRect(rect);
    if(_levelManager->isCurrentFieldOfView(rect))
    {
        //qDebug() << "I dont have to be reloaded";
        return;
    }

    Tiler tiler(rect, tileSize);
    tiler.exec();

    auto tiles = tiler.getTilingResult();
    for(int i = 0; i < tiles.size(); i++)
    {
        std::string key = std::to_string(tiles[i].getLevel()) + " " + std::to_string(tiles[i].getX()) + " " + std::to_string(tiles[i].getX());
        if(!_cache->contains(key))
        {
            _manager->addJob(tiles[i].getX() * viewSceneScale, tiles[i].getY() * viewSceneScale, level, tiles[i].getWidth(), tiles[i].getHeight(), JobType::ViewJob);
        } else
        {
            _cache->updateTile(key);
        }

    }
}



void AnnotatorController::levelChanged(QRectF rect)
{
    _levelManager->setCurrentLevel(rect);
}


std::shared_ptr<AnnotatorMainWindow> AnnotatorController::getMainWindow()
{
    return _window;
}

void AnnotatorController::showUI()
{
    if(!_window)
    {
        return;
    }

    _window->move(_window->screen()->geometry().center() - _window->frameGeometry().center());
    _window->show();
}

void AnnotatorController::connectActions()
{
    auto bar = _window->getToolBar();
    auto list = bar->actions();
    for(int i = 0; i < list.size(); i++)
    {
        if(list[i]->objectName() == "Panning")
        {
            connect(list[i], &QAction::triggered, this, &AnnotatorController::clickTriggered);
        }
        if(list[i]->objectName() == "Drawing")
        {
            connect(list[i], &QAction::triggered, this, &AnnotatorController::drawTriggered);
        }
        if(list[i]->objectName() == "Exit")
        {
            connect(list[i], &QAction::triggered, this, &AnnotatorController::exitTriggered);
        }
    }
}

void AnnotatorController::clickTriggered(bool checked)
{

}

void AnnotatorController::drawTriggered(bool checked)
{

}

void AnnotatorController::exitTriggered(bool checked)
{

    if(_manager)
    {
        _manager->shutdown();
    }
    if(_view)
    {
        _view->close();
        _view = nullptr;
    }

    if(_reader)
    {
        _window->statusBar()->showMessage("Closing");
        _reader->cleanUp();
    }

}

void AnnotatorController::itemLoaded(GraphicsItem *item)
{
    _cache->insertNewElement(item);
    qDebug() << _view->scene()->items().size();

}