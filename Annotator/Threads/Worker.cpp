//
// Created by Daniel Pietsch on 06.08.22.
//

#include "Worker.h"

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
            emit finished(job->getBuf(), job->getXPos(), job->getYPos(), job->getWidth(), job->getHeight(), job->getLevel());
            delete job;
        }

        _mutex.unlock();
    }
}

void Worker::abort()
{
    _abort = true;
}