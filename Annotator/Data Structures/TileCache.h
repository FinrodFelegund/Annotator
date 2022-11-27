//
// Created by Daniel Pietsch on 18.11.22.
//

#ifndef ANNOTATOR_TILECACHE_H
#define ANNOTATOR_TILECACHE_H
#include <unordered_map>
#include <QRectF>

class Tile;

class TileCache {

public:
    TileCache(int tileSize);
    ~TileCache();
    void resetCache();
    void setLevelCache(QRectF rect);
    QRectF toCacheRect(QRectF rect);
    QRectF getCurrentSceneLevel();
    bool isAlreadyLoaded(Tile tile);
    void setLoaded(Tile tile);
private:
    QRectF _currentSceneLevel;
    std::unordered_map<std::string, bool> _tiles;
    int _currentLevel;
    int _currentSceneScale;
    int _tileSize;
};


#endif //ANNOTATOR_TILECACHE_H
