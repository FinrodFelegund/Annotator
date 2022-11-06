//
// Created by Daniel Pietsch on 05.11.22.
//

#ifndef ANNOTATOR_TILE_H
#define ANNOTATOR_TILE_H


struct Tile
{
    int _xCoord;
    int _yCoord;
    int _width;
    int _height;
    Tile(int x, int y, int width, int height);
    int getX();
    int getY();
    int getHeight();
    int getWidth();
};


#endif //ANNOTATOR_TILE_H
