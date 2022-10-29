//
// Created by Daniel Pietsch on 06.08.22.
//

#include "TileJob.h"
#include <QMutex>

TileJob::TileJob(int x, int y, int width, int height, int lvl, std::shared_ptr<WholeSlideImageReader> reader)
{
    xPos = x;
    yPos = y;
    this->width = width;
    this->height = height;
    level = lvl;
    buf = nullptr;

    _reader = reader;
}

TileJob::~TileJob()
{
    if(buf)
    {
        delete buf;
        buf = nullptr;
    }

    _reader = nullptr;
}

void TileJob::setImage(std::shared_ptr<WholeSlideImageReader> reader)
{
    _reader = reader;
}

void TileJob::doJob()
{
    buf = (unsigned int*)_reader->readDataFromImage(xPos, yPos, width, height, level);
}

int TileJob::getXPos()
{
    return xPos;
}

int TileJob::getYPos()
{
    return yPos;
}

int TileJob::getWidth()
{
    return width;
}

int TileJob::getHeight()
{
    return height;
}

unsigned int *TileJob::getBuf()
{
    if(buf)
    {
        return buf;
    }

    return nullptr;
}

int TileJob::getLevel()
{
    return level;
}

