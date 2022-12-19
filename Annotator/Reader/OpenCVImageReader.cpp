//
// Created by Daniel Pietsch on 15.12.22.
//

#include "OpenCVImageReader.h"
#include <QDebug>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <QImage>


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

    _mat = cv::imread(imagePath, cv::IMREAD_COLOR);


    if(_mat.size().width == 0 || _mat.size().height == 0)
    {
        _valid = false;
        qDebug() << "Unable to open image";
        _errorState = "Image not valid";
        return;
    }

    cv::cvtColor(_mat, _mat, cv::COLOR_BGR2RGB);
    _filetype = imagePath;
    _valid = true;

    std::pair<int, int> dims = std::make_pair(_mat.size().width, _mat.size().height);
    _dimensions.push_back(dims);
    _scaleFactors.push_back(1);
    _numberOfLevels = 1;

    printLevelDownsample();
    printDimensions();
    //printProperties();
}

unsigned char* OpenCVImageReader::readDataFromImage(int64_t x, int64_t y, int64_t width, int64_t height, int32_t level)
{
    if(_valid)
    {
        std::shared_lock<std::shared_mutex> l(*_mutex);
        cv::Rect tile(x, y, width, height);
        unsigned char *buf = new unsigned char[width * height * 3];
        memcpy(buf, cv::Mat(_mat, tile).clone().data, width * height * 3);
        return buf;
    }

    return nullptr;
}

bool OpenCVImageReader::saveToDiskCV(std::string name, cv::Mat mat)
{
    return cv::imwrite(name, mat);
}

bool OpenCVImageReader::saveToDiskQt(std::string name, cv::Mat mat)
{
    cv::Size size = mat.size();
    QImage img((unsigned  char*)mat.data, size.width, size.height, mat.step1(), QImage::Format_RGB888);
    QString namer = QString::number(counter++) + "Test" + QString::number(size.width) + QString::number(size.height) + ".jpeg";
    return img.save(namer);
}