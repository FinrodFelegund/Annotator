//
// Created by Daniel Pietsch on 05.11.22.
//

#include "Tile.h"

Tile::Tile(qreal x, qreal y, int width, int height)
{
    _xCoord = x;
    _yCoord = y;
    _width = width;
    _height = height;
}

qreal Tile::getX()
{
    return _xCoord;
}

qreal Tile::getY()
{
    return _yCoord;
}

int Tile::getWidth()
{
    return _width;
}

int Tile::getHeight()
{
    return _height;
}
