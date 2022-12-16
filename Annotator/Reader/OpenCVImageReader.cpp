//
// Created by Daniel Pietsch on 15.12.22.
//

#include "OpenCVImageReader.h"
#include <QDebug>


OpenCVImageReader::OpenCVImageReader(std::string extension) : ImageReader(extension)
{
    _valid = false;
}

OpenCVImageReader::~OpenCVImageReader()
{
    cleanUp();
}

void OpenCVImageReader::cleanUp()
{
    _dimensions.clear();
    _scaleFactors.clear();
    _valid = false;

}

void OpenCVImageReader::initializeImage(const std::string imagePath)
{
    std::unique_lock<std::shared_mutex> lock(*_mutex);
    cleanUp();

    cv::Mat mat = cv::imread(imagePath, cv::IMREAD_COLOR);

    if(mat.size().width == 0 || mat.size().height == 0)
    {
        _valid = false;
        qDebug() << "Unable to open image";
        _errorState = "Image not valid";
        return;
    }
    _filetype = imagePath;
    _valid = true;

    std::pair<int, int> dims = std::make_pair(mat.size().width, mat.size().height);
    _dimensions.push_back(dims);
    _scaleFactors.push_back(1);
    _numberOfLevels = 1;

    printLevelDownsample();
    printDimensions();
    //printProperties();
}

unsigned char* OpenCVImageReader::readDataFromImage(int64_t x, int64_t y, int64_t width, int64_t height, int32_t level)
{   qDebug() << "X: " << x << " Y: " << y << " Width: " << width << " Height: " << height;
    if(_valid)
    {
        cv::Mat mat = cv::imread(_filetype, cv::IMREAD_COLOR);
        std::shared_lock<std::shared_mutex> l(*_mutex);
        cv::Mat croppedImage = mat(cv::Range(y, height), cv::Range(x, width));
        return (unsigned char*)croppedImage.data;
    }

    return nullptr;
}