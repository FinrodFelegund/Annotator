//
// Created by Daniel Pietsch on 04.11.22.
//

#ifndef ANNOTATOR_ANNOTATORCONTROLLER_H
#define ANNOTATOR_ANNOTATORCONTROLLER_H

#include <QObject>
#include "../AnnotatorUI/AnnotatorMainWindow.h"
#include "../Reader/WholeSlideImageReader.h"
#include "../Threads/Manager.h"
#include "../Data Structures/GraphicsItem.h"
#include "../Data Structures/ItemCache.h"

struct LevelManager
{

    LevelManager();
    ~LevelManager();
    void setCurrentLevel(QRectF levelRect);
    QRectF toLevelRect(QRectF fieldOfView);
    void setTileSize(int tileSize);
    bool isCurrentFieldOfView(QRectF rect);

    QRectF _currentLevelRect;
    QRectF _currentFieldOfView;
    int _tileSize;
};


class AnnotatorController : public QObject
{
    Q_OBJECT

public:
    AnnotatorController(QObject *parent = nullptr);
    ~AnnotatorController() noexcept;
    void shutDown();
    void setWindow(std::shared_ptr<AnnotatorMainWindow> window);
    std::shared_ptr<AnnotatorMainWindow> getMainWindow();
    void showUI();
    void connectActions();

public slots:
    void initializeImage(std::string fileName);
    void fieldOfViewChanged(QRectF rect);

    void clickTriggered(bool checked);
    void drawTriggered(bool checked);
    void exitTriggered(bool checked);
    void levelChanged(QRectF rect);
    void itemLoaded(GraphicsItem *item);


private:
    std::shared_ptr<AnnotatorMainWindow> _window;
    std::shared_ptr<WholeSlideImageReader> _reader;
    std::shared_ptr<AnnotatorViewer> _view;
    std::shared_ptr<Manager> _manager;
    std::shared_ptr<LevelManager> _levelManager;
    std::shared_ptr<ItemCache> _cache;


};


#endif //ANNOTATOR_ANNOTATORCONTROLLER_H
