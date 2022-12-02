//
// Created by Daniel Pietsch on 06.08.22.
//

#ifndef ANNOTATOR_MANAGER_H
#define ANNOTATOR_MANAGER_H

#include <QObject>
#include <QDebug>
#include <QWaitCondition>
#include "../Reader/WholeSlideImageReader.h"
#include "../Data Structures/TileJob.h"
#include <QMutex>

class Worker;

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = nullptr);
    ~Manager() noexcept;

private:
    std::list<TileJob*> _workList;
    std::vector<Worker*> _threadList;
    bool _stop;
    int _waitingThreads;
    QMutex _mutex;
    QWaitCondition _condition;
    std::shared_ptr<WholeSlideImageReader> _imageToRead;
    int _tileSize;
    int _numberOfThreads;
    int _jobsToDo;
    int _currentLevel;

public:
    void addJob(int x, int y, int level, int width, int height, JobType type);
    std::vector<Worker*> getThreads();
    std::list<TileJob*> getWork();
    void shutdown();
    void clearJobs();
    TileJob *getJob();
    int getWaitingThreads();
    void print();
    void setImage(std::shared_ptr<WholeSlideImageReader> img);
    int getNumberOfJobs();
    void startWorkers();
    void removeJob(QRectF rect, int level);
    int getCurrentLevel();
    void setCurrentLevel(int level);

};


#endif //ANNOTATOR_MANAGER_H
