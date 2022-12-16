//
// Created by Daniel Pietsch on 06.08.22.
//

#ifndef ANNOTATOR_TILEJOB_H
#define ANNOTATOR_TILEJOB_H
#include <memory>
#include "../Reader/ImageReader.h"
#include "Tile.h"

class QMutex;

enum JobType
{
    ViewJob,
    MinimapJob

};

struct TileJob
{
    TileJob(int x, int y, int width, int height, int lvl, std::shared_ptr<ImageReader> reader = nullptr, unsigned char *buf = nullptr, JobType type = JobType::ViewJob);
    ~TileJob();
    int xPos;
    int yPos;
    int width;
    int height;
    int level;
    unsigned char *buf = nullptr;
    JobType type;


    std::shared_ptr<ImageReader> _reader;

    void doJob();
    void setImage(std::shared_ptr<ImageReader> reader);
    int getXPos();
    int getYPos();
    int getWidth();
    int getHeight();
    int getLevel();
    unsigned char* getBuf();
    Tile toTile();
};




#endif //ANNOTATOR_TILEJOB_H
