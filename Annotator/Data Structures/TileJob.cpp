//
// Created by Daniel Pietsch on 06.08.22.
//

#include "TileJob.h"
#include <QMutex>
#include <QDebug>

TileJob::TileJob(int x, int y, int width, int height, int lvl, std::shared_ptr<ImageReader> reader, unsigned char *buf, JobType type)
{
    xPos = x;
    yPos = y;
    this->width = width;
    this->height = height;
    level = lvl;
    this->buf = buf;

    _reader = reader;
    this->type = type;
}

TileJob::~TileJob()
{
    if(buf)
    {
        //delete [] buf;
        buf = nullptr;
    }

    _reader = nullptr;
}

void TileJob::setImage(std::shared_ptr<ImageReader> reader)
{
    _reader = reader;
}

void TileJob::doJob()
{
    buf = _reader->readDataFromImage(xPos, yPos, width, height, level);
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

unsigned char *TileJob::getBuf()
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

Tile TileJob::toTile()
{
    Tile tile(xPos, yPos, width, height);
    tile.setBuf(buf);
    tile.setLevel(level);
    return tile;

}

