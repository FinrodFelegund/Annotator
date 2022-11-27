//
// Created by Daniel Pietsch on 06.08.22.
//

#ifndef ANNOTATOR_TILEJOB_H
#define ANNOTATOR_TILEJOB_H
#include <memory>
#include "../Reader/WholeSlideImageReader.h"
#include "Tile.h"

class QMutex;

enum JobType
{
    ViewJob,
    MinimapJob

};

struct TileJob
{
    TileJob(int x, int y, int width, int height, int lvl, std::shared_ptr<WholeSlideImageReader> reader = nullptr, unsigned char *buf = nullptr, JobType type = JobType::ViewJob);
    ~TileJob();
    int xPos;
    int yPos;
    int width;
    int height;
    int level;
    unsigned char *buf;
    JobType type;


    std::shared_ptr<WholeSlideImageReader> _reader;

    void doJob();
    void setImage(std::shared_ptr<WholeSlideImageReader> reader);
    int getXPos();
    int getYPos();
    int getWidth();
    int getHeight();
    int getLevel();
    unsigned char* getBuf();
    Tile toTile();
};




#endif //ANNOTATOR_TILEJOB_H
