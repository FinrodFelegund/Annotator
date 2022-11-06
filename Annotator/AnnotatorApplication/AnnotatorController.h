//
// Created by Daniel Pietsch on 04.11.22.
//

#ifndef ANNOTATOR_ANNOTATORCONTROLLER_H
#define ANNOTATOR_ANNOTATORCONTROLLER_H

#include <QObject>
#include "../AnnotatorUI/AnnotatorMainWindow.h"
#include "../Reader/WholeSlideImageReader.h"
#include "../Threads/Manager.h"


class AnnotatorController : public QObject
{
    Q_OBJECT

public:
    AnnotatorController(QObject *parent = nullptr);
    ~AnnotatorController();
    void shutDown();
    void setWindow(std::shared_ptr<AnnotatorMainWindow> window);
    std::shared_ptr<AnnotatorMainWindow> getMainWindow();
    void showUI();

public slots:
    void initializeImage(std::string fileName);
    void fieldOfViewChanged(QRect rect);


private:
    std::shared_ptr<AnnotatorMainWindow> _window;
    std::shared_ptr<WholeSlideImageReader> _reader;
    std::shared_ptr<AnnotatorViewer> _view;
    std::shared_ptr<Manager> _manager;

};


#endif //ANNOTATOR_ANNOTATORCONTROLLER_H
