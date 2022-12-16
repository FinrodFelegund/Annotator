//
// Created by Daniel Pietsch on 15.12.22.
//

#include "ImageReader.h"
#include <QDebug>

ImageReader::ImageReader(std::string extension)
{
    _mutex.reset(new std::shared_mutex());
    _extension = extension;
}

ImageReader::~ImageReader()
{
    cleanUp();
}

int ImageReader::getScaleFactor(int level)
{
    if(_scaleFactors.empty())
    {
        return 1;
    }
    return _scaleFactors[level];
}

bool ImageReader::isValid()
{
    return _valid;
}

std::string ImageReader::getProperty(const std::string property)
{
    if(_valid)
    {
        std::map<std::string, std::string>::iterator it = _propertiesMap.begin();
        while(it != _propertiesMap.end())
        {
            if(property == it->first)
            {
                return it->second;
            }
        }
    }
    return "";
}


std::string ImageReader::getFileType()
{
    if(_valid)
    {
        return _filetype;
    }
    return "";
}


double ImageReader::getLevelDownSample(const int level)
{

    if(_dimensions.empty())
    {
        return 1;
    }

    if(_valid && level < getNumberOfLevels())
    {
        double downSampleOnLevel0 = _dimensions[0].first;
        double downSampleOnLevel = _dimensions[level].first;

        return downSampleOnLevel0 / downSampleOnLevel;
    }
    return 0.;
}

int ImageReader::getLevelForGivenDownSample(double downSample)
{
    if(_valid)
    {
        if(downSample <= 1.)
        {
            return getNumberOfLevels() - 1;
        }

        int sample = getLevelDownSample(getNumberOfLevels() - 1);
        double DownSample = sample / downSample;

        if(DownSample <= 1.)
        {
            return 0;
        }

        for(int i  = _dimensions.size() - 2; i > 0; i--)
        {
            double nextDownSample = (double)_dimensions[0].first / (double)_dimensions[i].first;
            double currentDownSample = (double)_dimensions[0].first / (double)_dimensions[i + 1].first;

            if(DownSample <= currentDownSample && DownSample > nextDownSample)
            {
                return i;
            }

        }
        return 0;
    } else
    {
        return -1;
    }
}

int ImageReader::getNumberOfLevels()
{
    if(_valid)
    {
        return _numberOfLevels;
    }
    return -1;
}


std::string ImageReader::getErrorState()
{
    return _errorState;
}

std::pair<int, int> ImageReader::getLevel0Dimensions()
{
    if(_dimensions.empty())
    {
        return std::make_pair(0, 0);
    }
    if(_valid)
    {
        return _dimensions[0];
    }
    return std::make_pair(-1, -1);
}

std::pair<int, int> ImageReader::getLevelDimensions(const int level)
{
    if(_dimensions.empty())
    {
        return std::make_pair(0 ,0);
    }

    if(_valid && level < getNumberOfLevels())
    {
        return _dimensions[level];
    }
    return std::make_pair(-1, -1);
}


void ImageReader::printDimensions()
{
    if(_valid)
    {
        for(int i = 0; i < getNumberOfLevels(); i++)
        {
            qDebug() << "Level: " << i << " Width: " << _dimensions[i].first << " Height: " << _dimensions[i].second;
        }
    } else
    {
        qDebug() << "Invalid image";
    }
}

void ImageReader::printLevelDownsample()
{
    if(!_scaleFactors.empty())
    {
        for(int i = 0; i < _scaleFactors.size(); i++)
        {
            qDebug() << "Level: " << i;
            qDebug() << "Downsample: " << _scaleFactors[i];
            qDebug();
        }
    }
}

void ImageReader::printProperties()
{
    for(std::map<std::string, std::string>::iterator it = _propertiesMap.begin(); it != _propertiesMap.end(); it++)
    {
        qDebug() << QString::fromStdString(it->first) << ": " << QString::fromStdString(it->second);
    }
}

void ImageReader::cleanUp()
{
    _extension.clear();
    _dimensions.clear();
    _scaleFactors.clear();
    _numberOfLevels = 0;
    _filetype.clear();
    _propertiesMap.clear();
    _errorState.clear();
    _valid = false;

}


