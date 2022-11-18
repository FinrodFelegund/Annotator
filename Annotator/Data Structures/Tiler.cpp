//
// Created by Daniel Pietsch on 05.11.22.
//

#include "Tiler.h"
#include <QDebug>

Tiler::Tiler(QRectF rect, int tileSize)
{
    _rectangle = rect;
    _tileSize = tileSize;
}

Tiler::~Tiler()
{
    cleanUp();
}

void Tiler::cleanUp()
{
    _tiles.clear();
}

std::vector<Tile> Tiler::getTilingResult()
{
    return _tiles;
}

void Tiler::exec()
{
    int tilesX = qCeil(_rectangle.width() / qreal(_tileSize));
    int tilesY = qCeil(_rectangle.height() / qreal(_tileSize));

    int width, height = 0;

    for(int i = 0; i < tilesX; i++)
    {
        for(int j = 0; j < tilesY; j++)
        {
            if(i == tilesX - 1)
            {
                width = (_rectangle.width() - i * _tileSize);
            } else
            {
                width = _tileSize;
            }

            if(j == tilesY - 1)
            {
                height = (_rectangle.height() - j * _tileSize);
            } else
            {
                height = _tileSize;
            }

            Tile tile(_rectangle.x() + i * _tileSize, _rectangle.y() + j * _tileSize, width, height);
            _tiles.push_back(tile);

        }
    }
}

void Tiler::print()
{
    for(int i = 0; i < _tiles.size(); i++)
    {
        qDebug() << _tiles[i].getX() << " " << _tiles[i].getY() << " " << _tiles[i].getWidth() << " " << _tiles[i].getHeight();
    }
}
