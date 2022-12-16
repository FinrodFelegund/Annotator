//
// Created by Daniel Pietsch on 15.12.22.
//

#ifndef ANNOTATOR_IMAGEREADER_H
#define ANNOTATOR_IMAGEREADER_H
#include <vector>
#include <string>
#include <shared_mutex>
#include <map>


class ImageReader {

public:
    ImageReader(std::string extension);
    ~ImageReader();
    virtual void cleanUp();
    virtual void initializeImage(const std::string imagePath) = 0;
    virtual unsigned char *readDataFromImage(int64_t x, int64_t y, int64_t width, int64_t height, int32_t level) = 0;
    std::string getFileType();
    int getNumberOfLevels();
    std::string getProperty(std::string property);
    bool isValid();
    double getLevelDownSample(const int level);
    std::pair<int, int> getLevelDimensions(const int level);
    std::pair<int, int> getLevel0Dimensions();
    std::string getErrorState();
    void printDimensions();
    void printLevelDownsample();
    void printProperties();
    int getScaleFactor(int level);
    int getLevelForGivenDownSample(double level);

protected:
    std::vector<std::pair<int, int>> _dimensions;
    std::vector<int> _scaleFactors;
    int _numberOfLevels;
    const int _tileSize = 1024;
    std::shared_ptr<std::shared_mutex> _mutex;
    std::string _filetype;
    std::string _extension;
    std::map<std::string, std::string> _propertiesMap;
    std::string _errorState;
    bool _valid;

};


#endif //ANNOTATOR_IMAGEREADER_H
