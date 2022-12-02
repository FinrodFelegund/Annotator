//
// Created by Daniel Pietsch on 06.08.22.
//

#include "Manager.h"
#include "Worker.h"

Manager::Manager(QObject *parent) : QObject(parent)
{
    _stop = false;
    _waitingThreads = 0;
    _imageToRead = nullptr;
    _tileSize = 0;
    _currentLevel = 0;

    _numberOfThreads = std::thread::hardware_concurrency();


}

void Manager::startWorkers()
{
    _stop = false;
    for(int i = 0; i < _numberOfThreads; i++)
    {
        Worker *worker = new Worker(this);
        worker->start(QThread::HighPriority);
        _threadList.push_back(worker);
    }
}

Manager::~Manager() noexcept
{
    shutdown();
}

void Manager::shutdown()
{
    _stop = true;
    //qDebug() << "Number of threads to kill: " << _threadList.size();
    for(std::vector<Worker*>::iterator it = _threadList.begin(); it != _threadList.end(); ++it)
    {
        (*it)->abort();
        while((*it)->isRunning())
        {
            _condition.wakeOne();
            //qDebug() << "Wake One";
        }
        //qDebug() << "delete";
        delete (*it);

    }
    clearJobs();
    _threadList.clear();

}

void Manager::clearJobs()
{
    QMutexLocker locker(&_mutex);
    for(auto job : _workList)
    {
        delete job;
    }
    _workList.clear();
}

void Manager::addJob(int x, int y, int level, int width, int height, JobType type)
{
    QMutexLocker locker(&_mutex);
    if(!_imageToRead)
    {
        return;
    }
    TileJob *tile = new TileJob(x, y, width, height, level, nullptr, nullptr, type);
    tile->setImage(_imageToRead);
    _workList.push_front(tile);
    _condition.wakeOne();
}

int Manager::getNumberOfJobs()
{
    return _workList.size();
}

void Manager::setImage(std::shared_ptr<WholeSlideImageReader> img)
{
    _imageToRead = img;
}

std::vector<Worker*> Manager::getThreads()
{
    return _threadList;
}

std::list<TileJob*> Manager::getWork()
{
    return _workList;
}

TileJob* Manager::getJob()
{
    _mutex.lock();
    while(_workList.empty() && !_stop)
    {
        _waitingThreads++;
        _condition.wait(&_mutex);
        _waitingThreads--;
    }

    if(_stop)
    {
        _mutex.unlock();
        return nullptr;
    }

    TileJob *tile = _workList.front();
    _workList.pop_front();
    _mutex.unlock();
    return tile;
}


int Manager::getWaitingThreads()
{
    return _waitingThreads;
}

void Manager::removeJob(QRectF rect, int level)
{
    if(!_imageToRead)
    {
        return;
    }

    _mutex.lock();
    int downSample = _imageToRead->getScaleFactor(level);
    rect = QRectF(rect.x() * downSample, rect.y() * downSample, rect.width() * downSample, rect.height() * downSample);
    std::list<TileJob*>::iterator it;
    for(it = _workList.begin(); it != _workList.end(); it++)
    {
        int sample = _imageToRead->getScaleFactor((*it)->getLevel());
        QRectF del((*it)->getXPos() * sample, (*it)->getYPos() * sample, (*it)->getWidth(), (*it)->getHeight());
        qDebug() << del;
        if(del.intersects(rect) && level != (*it)->getLevel())
        {
            _workList.erase(it);
        }
    }


    _mutex.unlock();
}

void Manager::setCurrentLevel(int level)
{
    QMutexLocker locker(&_mutex);
    _currentLevel = level;

}

int Manager::getCurrentLevel()
{
    QMutexLocker locker(&_mutex);
    qDebug() << "residing here";
    return _currentLevel;
}





