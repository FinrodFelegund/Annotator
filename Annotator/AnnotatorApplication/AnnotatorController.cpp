//
// Created by Daniel Pietsch on 04.11.22.
//

#include "AnnotatorController.h"
#include <QScreen>
#include "../Data Structures/Tiler.h"
#include "../Data Structures/Tile.h"
#include "../Threads/Worker.h"


AnnotatorController::AnnotatorController(QObject *parent) : QObject(parent)
{
    _window = nullptr;
    _reader = std::make_shared<WholeSlideImageReader>();
    _manager = std::make_shared<Manager>(this);

}

AnnotatorController::~AnnotatorController() noexcept
{
    shutDown();
}

void AnnotatorController::shutDown()
{
    if(_window)
    {
        _window = nullptr;
    }

    if(_reader)
    {
        //_reader->cleanUp();
        _reader = nullptr;
    }

    if(_view)
    {
        //_view->close();
        _view = nullptr;
    }

}

void AnnotatorController::setWindow(std::shared_ptr<AnnotatorMainWindow> window)
{
    if(!window)
    {
        return;
    }

    _window = window;
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
    _manager->setImage(_reader);
    _view = _window->getView();

    if(!_view)
    {
        qDebug() << "AnnotatorController: view is not valid";
        return;
    }

    std::vector<Worker*> worker = _manager->getThreads();
    for(int i = 0; i < worker.size(); i++)
    {
        connect(worker[i], &Worker::finished, _view.get(), &AnnotatorViewer::loadTileInScene);
    }

    connect(_view.get(), &AnnotatorViewer::fieldOfViewChanged, this, &AnnotatorController::fieldOfViewChanged);

    _view->initialize(_reader);
    int viewSceneScale = _view->getCurrentSceneScale();
    int level = _view->getCurrentLevel();
    int tileSize = _view->getTileSize();
    QRect sceneRect = _view->getSceneRect().toRect();
    sceneRect.setWidth(qCeil(sceneRect.width() / viewSceneScale));
    sceneRect.setHeight(qCeil(sceneRect.height() / viewSceneScale));
    Tiler tiler(sceneRect, tileSize);
    tiler.exec();
    auto tiles = tiler.getTilingResult();
    for(int i = 0; i < tiles.size(); i++)
    {
        _manager->addJob(tiles[i].getX() * viewSceneScale, tiles[i].getY() * viewSceneScale, level, tiles[i].getWidth(), tiles[i].getHeight());
    }


}

void AnnotatorController::fieldOfViewChanged(QRect rect)
{
    qDebug() << "X: " << rect.x() << " Y: " << rect.y() << " width: " << rect.width() << " height" << rect.height();
    int viewSceneScale = _view->getCurrentSceneScale();
    int tileSize = _view->getTileSize();
    int level = _view->getCurrentLevel();
    Tiler tiler(rect, tileSize);
    tiler.exec();
    auto tiles = tiler.getTilingResult();

    for(int i = 0; i < tiles.size(); i++)
    {
        _manager->addJob(tiles[i].getX() * viewSceneScale, tiles[i].getY() * viewSceneScale, level, tiles[i].getWidth(), tiles[i].getHeight());
    }
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