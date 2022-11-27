//
// Created by Daniel Pietsch on 06.08.22.
//

#ifndef ANNOTATOR_WORKER_H
#define ANNOTATOR_WORKER_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <QMutex>
#include "Manager.h"
#include "../Reader/WholeSlideImageReader.h"

class QPixmap;
class Tile;

class Worker : public QThread
{
    Q_OBJECT
public:
    Worker(Manager *parent);
    ~Worker();

    void run() Q_DECL_OVERRIDE;
    void abort();

signals:

    void finished(Tile tile);
    void finishedMinimapJob(Tile tile);

private:

    QMutex _mutex;
    bool _abort;

};


#endif //ANNOTATOR_WORKER_H
