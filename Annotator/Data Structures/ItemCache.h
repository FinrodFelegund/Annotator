//
// Created by Daniel Pietsch on 02.12.22.
//

#ifndef ANNOTATOR_ITEMCACHE_H
#define ANNOTATOR_ITEMCACHE_H
#include <iostream>
#include <unordered_map>
#include <list>
#include "GraphicsItem.h"
#include "../Reader/WholeSlideImageReader.h"
#include "Tiler.h"
#include <QGraphicsScene>


class ItemCache {
public:
    ItemCache(QGraphicsScene *scene = nullptr);
    ~ItemCache();
    void clear();
    void setScene(QGraphicsScene *scene);
    void setTopLevel(int level);
    void insertNewElement(GraphicsItem *item);
    bool contains(std::string key);
    void updateTile(std::string key);
    void deleteFromScene(GraphicsItem *item);
    void printSizes();
    int getCurrentSize();


private:
    // level ( tile, loaded)
    std::unordered_map<std::string, std::pair<GraphicsItem*, std::list<std::string>::iterator>> _map;
    std::list<std::string> _list;
    int _currentSize;
    QGraphicsScene *_scene;
    const int _maxSize = 400;
    int _excludedLevel;
};


#endif //ANNOTATOR_ITEMCACHE_H
