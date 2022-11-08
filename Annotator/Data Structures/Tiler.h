//
// Created by Daniel Pietsch on 05.11.22.
//

#ifndef ANNOTATOR_TILER_H
#define ANNOTATOR_TILER_H
#include <QRect>
#include <vector>
#include "Tile.h"
#include <QtMath>

class Tiler {
public:
    Tiler(QRectF rect, int tileSize);
    ~Tiler();
    void exec();
    std::vector<Tile> getTilingResult();
    void cleanUp();
    void print();
private:
    QRectF _rectangle;
    int _tileSize;
    std::vector<Tile> _tiles;
};


#endif //ANNOTATOR_TILER_H
