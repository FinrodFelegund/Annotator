//
// Created by Daniel Pietsch on 15.12.22.
//

#include "ImageFactory.h"
#include <QDebug>
#include "WholeSlideImageReader.h"
#include "OpenCVImageReader.h"

ImageFactory::ImageFactory()
{

}

ImageFactory::~ImageFactory()
{

}

std::shared_ptr<ImageReader> ImageFactory::getImageTyp(std::string path)
{
    std::string imageType;
    std::string::iterator it = path.end();
    while((*it) != '.' && it != path.begin())
    {
        it--;
    }

    if(it == path.begin())
    {
        qDebug() << "Not a valid filetype";
        return nullptr;
    }

    imageType.insert(imageType.begin(), it, path.end());

    if(_supportedExtensions.find(imageType) == _supportedExtensions.end())
    {
        qDebug() << "This extension is not supported";
        return nullptr;
    }

    std::map<std::string, int>::iterator iter = _supportedExtensions.find(imageType);
    qDebug() << QString::fromStdString(imageType);

    switch(iter->second)
    {
        case 0: return std::make_shared<OpenCVImageReader>(imageType);
        case 1: return std::make_shared<WholeSlideImageReader>(imageType);
        default: return nullptr;
    }


}
