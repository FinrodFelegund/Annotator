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

class Worker : public QThread
{
    Q_OBJECT
public:
    Worker(Manager *parent);
    ~Worker();

    void run() Q_DECL_OVERRIDE;
    void abort();

signals:

    void finished(unsigned char* buffer, int x, int y, int width, int height, int level);

private:

    QMutex _mutex;
    bool _abort;

};


#endif //ANNOTATOR_WORKER_H
