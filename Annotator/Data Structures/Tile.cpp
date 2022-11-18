//
// Created by Daniel Pietsch on 05.11.22.
//

#include "Tile.h"

Tile::Tile(int x, int y, int width, int height)
{
    _xCoord = x;
    _yCoord = y;
    _width = width;
    _height = height;
    _buf = nullptr;
}

Tile::Tile()
{
    _buf = nullptr;
}

Tile::~Tile()
{

}



int Tile::getX()
{
    return _xCoord;
}

int Tile::getY()
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

void Tile::setBuf(unsigned char *buf)
{
    this->_buf = new unsigned char [_width * _height * 3];
    memcpy(this->_buf, buf, _width * _height * 3);
}

unsigned  char* Tile::getBuf()
{
    return _buf;
}

void Tile::setLevel(int level)
{
    this->_level = level;
}

int Tile::getLevel()
{
    return this->_level;
}
