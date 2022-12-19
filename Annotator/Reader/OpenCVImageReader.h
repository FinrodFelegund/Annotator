//
// Created by Daniel Pietsch on 15.12.22.
//

#ifndef ANNOTATOR_OPENCVIMAGEREADER_H
#define ANNOTATOR_OPENCVIMAGEREADER_H
#include "ImageReader.h"
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <map>



class OpenCVImageReader : public ImageReader
{
public:
    OpenCVImageReader(std::string extension);
    ~OpenCVImageReader();
    void cleanUp();
    void initializeImage(const std::string imagePath);
    unsigned char *readDataFromImage(int64_t x, int64_t y, int64_t width, int64_t height, int32_t level);
    bool saveToDiskCV(std::string name, cv::Mat mat);
    bool saveToDiskQt(std::string name, cv::Mat mat);

private:
    cv::Mat _mat;
    int counter = 0;

};


#endif //ANNOTATOR_OPENCVIMAGEREADER_H
