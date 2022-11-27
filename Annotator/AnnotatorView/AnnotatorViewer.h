//
// Created by Daniel Pietsch on 08.08.22.
//

#ifndef ANNOTATOR_ANNOTATORVIEWER_H
#define ANNOTATOR_ANNOTATORVIEWER_H
#include <QGraphicsView>
#include <QScrollBar>
#include "../Threads/Manager.h"
#include "../Reader/WholeSlideImageReader.h"
#include <QFormLayout>
#include "MiniMap.h"

class Tile;

class AnnotatorViewer : public QGraphicsView
{
    Q_OBJECT
public:
    AnnotatorViewer(QObject *parent = nullptr);
    ~AnnotatorViewer() noexcept;
    void initialize(std::shared_ptr<WholeSlideImageReader> reader);
    void close();
    QRectF getSceneRect();
    int getCurrentLevel();
    qreal getCurrentSceneScale();
    int getTileSize();



private:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    void setMinimap();
    void setUI();

    std::shared_ptr<WholeSlideImageReader> _reader;
    qreal _currentSceneScale;
    int _tileSize;
    int _currentLevel;
    std::pair<int, int> _levelZeroDimensions;
    std::pair<int, int> _levelTopDimensions;
    bool _clicked;
    QPointF _currentPos;
    QRectF _currentScene;
    MiniMap *_map;



signals:
    void fieldOfViewChanged(QRectF rect);
    void fieldOfViewForMinimapChanged(QRectF rect);
    void levelChanged(QRectF rect);

public slots:
    void loadTileInScene(Tile tile);
    void centerOnMinimap(QPoint point);

};


#endif //ANNOTATOR_ANNOTATORVIEWER_H
