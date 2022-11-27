//
// Created by Daniel Pietsch on 18.11.22.
//

#include "TileCache.h"
#include "Tiler.h"
#include "Tile.h"
#include <QDebug>

TileCache::TileCache(int tileSize)
{
    _tileSize = tileSize;
}

TileCache::~TileCache()
{
    resetCache();
}

void TileCache::resetCache()
{
    _currentSceneScale = _currentLevel = 0;
    _tiles.clear();
    _currentSceneLevel = QRectF();
}

void TileCache::setLevelCache(QRectF rect)
{
    _currentSceneLevel = rect;
    Tiler tiler(_currentSceneLevel, _tileSize);
    tiler.exec();
    auto tiles = tiler.getTilingResult();
    for(int i = 0; i < tiles.size(); i++)
    {
        std::string st = std::to_string(tiles[i].getX()) + " " + std::to_string(tiles[i].getY());
        _tiles[st] = false;
    }

    qDebug() << "New level cache x: " << _currentSceneLevel.x() << " y: " << _currentSceneLevel.y() << " width: " << _currentSceneLevel.width() << " height: " << _currentSceneLevel.height();

}

QRectF TileCache::toCacheRect(QRectF rect)
{
    //get the next smaller (x,y) floating above rect
    int x = 0, y = 0;
    x = (int)rect.x() / _tileSize;
    y = (int)rect.y() / _tileSize;
    x = x * _tileSize;
    y = y * _tileSize;


    //get the width and height offset by tilesize
    int width = 0, height = 0;
    width = (int)rect.bottomRight().x() / _tileSize;
    height = (int)rect.bottomRight().y() / _tileSize;
    width = (width * _tileSize) + _tileSize;
    height = (height * _tileSize) + _tileSize;
    if(width > _currentSceneLevel.width())
    {
        width = _currentSceneLevel.width();
    }

    if(height > _currentSceneLevel.height())
    {
        height = _currentSceneLevel.height();
    }

    return QRectF(x, y, width - x, height - y);
}

bool TileCache::isAlreadyLoaded(Tile tile)
{
    std::string st = std::to_string(tile.getX()) + " " + std::to_string(tile.getY());
    return _tiles[st];
}

void TileCache::setLoaded(Tile tile)
{
    std::string st = std::to_string(tile.getX()) + " " + std::to_string(tile.getY());
    _tiles[st] = true;
}

QRectF TileCache::getCurrentSceneLevel()
{
    return _currentSceneLevel;
}



