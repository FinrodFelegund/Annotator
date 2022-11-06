//
// Created by Daniel Pietsch on 06.08.22.
//

#ifndef ANNOTATOR_TILEJOB_H
#define ANNOTATOR_TILEJOB_H
#include <memory>
#include "../Reader/WholeSlideImageReader.h"

class QMutex;

struct TileJob
{
    TileJob(int x, int y, int width, int height, int lvl, std::shared_ptr<WholeSlideImageReader> reader = nullptr, unsigned int *buf = nullptr);
    ~TileJob();
    int xPos;
    int yPos;
    int width;
    int height;
    int level;
    unsigned int *buf;

    std::shared_ptr<WholeSlideImageReader> _reader;

    void doJob();
    void setImage(std::shared_ptr<WholeSlideImageReader> reader);
    int getXPos();
    int getYPos();
    int getWidth();
    int getHeight();
    int getLevel();
    unsigned int* getBuf();
};




#endif //ANNOTATOR_TILEJOB_H
