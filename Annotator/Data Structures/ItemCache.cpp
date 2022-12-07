//
// Created by Daniel Pietsch on 02.12.22.
//

#include "ItemCache.h"

ItemCache::ItemCache(QGraphicsScene *scene)
{
    if(scene)
    {
        _scene = scene;
    }
    _currentSize = 0;
}

ItemCache::~ItemCache()
{
    clear();
}

void ItemCache::clear()
{
    if(_scene)
    {
        _scene = nullptr;
    }

    _excludedLevel = 0;
    _map.clear();
    _list.clear();
}

void ItemCache::setScene(QGraphicsScene *scene)
{
    if(scene)
    {
        _scene = scene;
    }
}

void ItemCache::insertNewElement(GraphicsItem *item)
{
    if(item->getLevel() != _excludedLevel)
    {

        std::string key = std::to_string(item->getLevel()) + " " + std::to_string(item->getX()) + " " + std::to_string(item->getY());
        if(_map.find(key) != _map.end())
        {
            //update LRU
            std::unordered_map<std::string, std::pair<GraphicsItem*, std::list<std::string>::iterator>>::iterator it = _map.find(key);
            _list.splice(_list.begin(), _list, it->second.second); // this brings the item to the front
            deleteFromScene(it->second.first);
            _map.erase(it);
            _map.insert(std::make_pair(key, std::make_pair(item, _list.begin())));

        } else
        {
            //insert the new item
            int size = _list.size();
            if(_currentSize >= _maxSize)
            {
                //if maxsize is reached, purge the last record
                std::unordered_map<std::string, std::pair<GraphicsItem*, std::list<std::string>::iterator>>::iterator it = _map.find(_list.back());
                auto item = it->second.first;
                deleteFromScene(item);
                delete it->second.first;
                it->second.first = nullptr;
                _list.pop_back();
                _map.erase(it);
                _currentSize--;


            }
            auto it = _list.insert(_list.begin(), key);
            _map.insert(std::make_pair(key, std::make_pair(item, it)));
            _currentSize++;


        }

    }
}

void ItemCache::updateTile(std::string key)
{
    std::unordered_map<std::string, std::pair<GraphicsItem*, std::list<std::string>::iterator>>::iterator it = _map.find(key);
    it->second.first->setOpacity(1.);
    it->second.first->update();

    _list.splice(_list.begin(), _list, it->second.second); // this brings the item to the front
    _map.erase(it);
    _map.insert(std::make_pair(key, std::make_pair(it->second.first, _list.begin())));


}

bool ItemCache::contains(std::string key)
{
    if(_map.find(key) == _map.end())
    {
        return false;
    }

    return true;
}

void ItemCache::deleteFromScene(GraphicsItem *item)
{
    if(item)
    {
        if(_scene)
        {
            _scene->removeItem(item);
        }
    }
}

void ItemCache::printSizes()
{
    qDebug() << "Mapsize: " << _map.size() << " Listsize: " << _list.size();
}

void ItemCache::setTopLevel(int level)
{
    _excludedLevel = level;
}

int ItemCache::getCurrentSize()
{
    return _currentSize;
}




