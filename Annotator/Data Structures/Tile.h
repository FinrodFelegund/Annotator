//
// Created by Daniel Pietsch on 05.11.22.
//

#ifndef ANNOTATOR_TILE_H
#define ANNOTATOR_TILE_H
#include <QtMath>


struct Tile
{
    qreal _xCoord;
    qreal _yCoord;
    int _width;
    int _height;
    Tile(qreal x, qreal y, int width, int height);
    qreal getX();
    qreal getY();
    int getHeight();
    int getWidth();
};


#endif //ANNOTATOR_TILE_H
