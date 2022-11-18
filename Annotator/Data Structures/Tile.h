//
// Created by Daniel Pietsch on 05.11.22.
//

#ifndef ANNOTATOR_TILE_H
#define ANNOTATOR_TILE_H
#include <QtMath>


struct Tile
{
    int _xCoord;
    int _yCoord;
    int _width;
    int _height;
    int _level;
    unsigned char *_buf;
    Tile(int x, int y, int width, int height);
    Tile();
    ~Tile();
    int getX();
    int getY();
    int getHeight();
    int getWidth();
    int getLevel();
    unsigned char* getBuf();
    void setBuf(unsigned char *buf);
    void setLevel(int level);
};


#endif //ANNOTATOR_TILE_H
