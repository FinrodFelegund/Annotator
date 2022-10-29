//
// Created by Daniel Pietsch on 03.08.22.
//

#ifndef ANNOTATOR_WHOLESLIDEIMAGEREADER_H
#define ANNOTATOR_WHOLESLIDEIMAGEREADER_H
#include <vector>
#include "openslide.h"
#include <string>
#include <iostream>
#include <shared_mutex>
#include <cmath>
#include <QGraphicsScene>

class WholeSlideImageReader {
public:
    WholeSlideImageReader();
    ~WholeSlideImageReader();

private:
    openslide_t *_obj;
    std::vector<std::pair<int, int>> _dimensions;
    std::vector<int> _scaleFactors;
    int _numberOfLevels;
    const int _tileSize = 1024;
    std::unique_ptr<std::shared_mutex> _mutex;
    std::string _filetype;
    std::map<std::string, std::string> _propertiesMap;
    std::string _errorState;
    bool _valid;
    int _bg_r;
    int _bg_g;
    int _bg_b;

public:
    void initializeImage(const std::string imagePath);
    void cleanUp();
    std::string getFileType();
    double getLevelDownSample(const int level);
    int getLevelForGivenDownSample(const double downSample);
    std::pair<int, int> getLevelDimensions(const int level);
    std::pair<int, int> getLevel0Dimensions();
    int getNumberOfLevels();
    std::string getProperty(const std::string property);
    bool isValid();
    std::string getErrorState();

    void *readDataFromImage(int64_t x, int64_t y, int64_t width, int64_t height, int32_t level);
    void printDimensions();
    void printLevelDownsample();

};


#endif //ANNOTATOR_WHOLESLIDEIMAGEREADER_H
