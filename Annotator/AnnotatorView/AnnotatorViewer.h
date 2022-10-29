//
// Created by Daniel Pietsch on 08.08.22.
//

#ifndef ANNOTATOR_ANNOTATORVIEWER_H
#define ANNOTATOR_ANNOTATORVIEWER_H
#include <QGraphicsView>
#include "../Threads/Manager.h"
#include "../Reader/WholeSlideImageReader.h"

class AnnotatorViewer : public QGraphicsView
{
    Q_OBJECT
public:
    AnnotatorViewer(QObject *parent = nullptr);
    ~AnnotatorViewer() noexcept;
    void initialize(std::shared_ptr<WholeSlideImageReader> reader);
    void close();
    void loadAllTilesForLevel(int level);
    void loadTilesForFieldOfView(QRect view, int level);

private:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);



private:
    std::shared_ptr<Manager> _manager;
    std::shared_ptr<WholeSlideImageReader> _reader;
    int _sceneScale;
    int _tileSize;
    int _lastLevel;
    int _currentScaleFactor;

private slots:
    void loadTileInScene(unsigned int *buf, int x, int y, int width, int height, int level);
};


#endif //ANNOTATOR_ANNOTATORVIEWER_H
