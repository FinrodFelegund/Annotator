//
// Created by Daniel Pietsch on 06.08.22.
//

#include "Worker.h"
#include "../Data Structures/Tile.h"

Worker::Worker(Manager *parent)
{
    setParent(parent);
    _abort = false;

}

Worker::~Worker()
{
    wait();
}

void Worker::run()
{
    for(;;)
    {
        TileJob *job = dynamic_cast<Manager*>(parent())->getJob();
        if(_abort)
        {
            return;
        }
        _mutex.lock();
        if(job)
        {
            job->doJob();
            Tile tile = job->toTile();
            emit finished(tile);
            delete job;
        }

        _mutex.unlock();
    }
}

void Worker::abort()
{
    _abort = true;
}