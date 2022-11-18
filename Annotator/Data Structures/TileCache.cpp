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
    _currentSceneLevel.setX(0);
    _currentSceneLevel.setY(0);
    _currentSceneLevel.setWidth(0);
    _currentSceneLevel.setHeight(0);
}

void TileCache::setLevelCache(QRectF rect)
{
    _currentSceneLevel = rect;
    Tiler tiler(rect, _tileSize);
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
    int x = 0, y = 0;
    bool loadedX = false, loadedY = false;

    //find out x, y Position of new rect
    while(!loadedX || !loadedY)
    {
        if(x + _tileSize < rect.x())
        {
            x += _tileSize;
        } else
        {
            loadedX = true;
        }

        if(y + _tileSize < rect.y())
        {
            y += _tileSize;
        } else
        {
            loadedY = true;
        }

    }

    //find out width and length of new rect
    loadedY = loadedX = false;
    int bottomRightX = rect.bottomRight().x();
    //bottomRightX = bottomRightX
    int bottomRightY = rect.bottomRight().y();
    int width = x;
    int height = y;
    while(!loadedX || !loadedY)
    {
        if(width >= bottomRightX)
        {
            loadedX = true;
        } else
        {
            width += _tileSize;
        }

        if(height >= bottomRightY)
        {
            loadedY = true;
        } else
        {
            height += _tileSize;
        }
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



