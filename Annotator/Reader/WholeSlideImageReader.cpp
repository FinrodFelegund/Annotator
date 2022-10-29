//
// Created by Daniel Pietsch on 03.08.22.
//

#include "WholeSlideImageReader.h"
#include <QImage>
#include <QLabel>

WholeSlideImageReader::WholeSlideImageReader() {

    _obj = nullptr;
    _numberOfLevels = 0;
    _errorState = "";
    _filetype = "";
    _valid = false;
    _bg_r = 255;
    _bg_g = 255;
    _bg_b = 255;

    _mutex.reset(new std::shared_mutex());
}

WholeSlideImageReader::~WholeSlideImageReader()
{
    std::unique_lock<std::shared_mutex> lock(*_mutex);
    cleanUp();
}

void WholeSlideImageReader::initializeImage(const std::string imagePath)
{
    std::unique_lock<std::shared_mutex> lock(*_mutex);

    cleanUp();
    _obj = openslide_open(imagePath.c_str());

    if(!_obj)
    {
        return;
    }

    if(const char *error = openslide_get_error(_obj))
    {
        _errorState = error;
    }

    if(_errorState.empty()) {
        _numberOfLevels = openslide_get_level_count(_obj);
        for (int i = 0; i < _numberOfLevels; i++) {
            int64_t x, y = 0;
            openslide_get_level_dimensions(_obj, i, &x, &y);
            std::pair<int, int> dims = std::make_pair(x, y);
            _dimensions.push_back(dims);
            _scaleFactors.push_back(openslide_get_level_downsample(_obj, i));
        }

        //printLevelDownsample();

        if(const char *const *value = openslide_get_property_names(_obj))
        {
            _propertiesMap.insert(std::make_pair("Name", std::string(*value)));
        }

        if (const char *value = openslide_get_property_value(_obj, OPENSLIDE_PROPERTY_NAME_MPP_X))
        {
            _propertiesMap.insert(std::make_pair("MPP_X", std::string(value)));
        }

        if (const char *value = openslide_get_property_value(_obj, OPENSLIDE_PROPERTY_NAME_MPP_Y))
        {
            _propertiesMap.insert(std::make_pair("MPP_Y", std::string(value)));
        }

        _filetype = openslide_get_property_value(_obj, OPENSLIDE_PROPERTY_NAME_VENDOR);

        const char *value = openslide_get_property_value(_obj, OPENSLIDE_PROPERTY_NAME_BACKGROUND_COLOR);
        if(value)
        {
            unsigned int bg_color = std::stoi(value, 0, 16);
            _bg_r = ((bg_color >> 16) & 0xff);
            _bg_g = ((bg_color >> 8) & 0xff);
            _bg_b = (bg_color & 0xff);
        }

        _valid = true;
    } else
    {
        _valid = false;
    }
}

void WholeSlideImageReader::cleanUp()
{
    if(_obj)
    {
        openslide_close(_obj);
        _obj = nullptr;
    }

    _dimensions.clear();
    _scaleFactors.clear();
    _numberOfLevels = 0;
    _filetype = "";
    _propertiesMap.clear();
    _errorState = "";
    _valid = false;
}

bool WholeSlideImageReader::isValid()
{
    return _valid;
}

std::string WholeSlideImageReader::getProperty(const std::string property)
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

std::string WholeSlideImageReader::getFileType()
{
    if(_valid)
    {
        return _filetype;
    }
    return "";
}

double WholeSlideImageReader::getLevelDownSample(const int level)
{
    if(_valid && level < getNumberOfLevels())
    {
        double downSampleOnLevel0 = _dimensions[0].first;
        double downSampleOnLevel = _dimensions[level].first;

        return downSampleOnLevel0 / downSampleOnLevel;
    }
    return 0.;
}

int WholeSlideImageReader::getLevelForGivenDownSample(const double downSample)
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

        for(int i  = _dimensions.size() - 1; i > 0; i--)
        {
            double currentDownSample = (double)_dimensions[0].first / (double)_dimensions[i].first;
            double nextDownSample = (double)_dimensions[0].first / (double)_dimensions[i - 1].first;

            if(DownSample <= currentDownSample && DownSample > nextDownSample)
            {
                return i;
            }

        }
        return getNumberOfLevels() - 1;
    } else
    {
        return -1;
    }
}

int WholeSlideImageReader::getNumberOfLevels()
{
    if(_valid)
    {
        return _numberOfLevels;
    }
    return -1;
}

std::string WholeSlideImageReader::getErrorState()
{
    return _errorState;
}

std::pair<int, int> WholeSlideImageReader::getLevel0Dimensions()
{
    if(_valid)
    {
        return _dimensions[0];
    }
    return std::make_pair(-1, -1);
}

std::pair<int, int> WholeSlideImageReader::getLevelDimensions(const int level)
{
    if(_valid && level < getNumberOfLevels())
    {
        return _dimensions[level];
    }
    return std::make_pair(-1, -1);
}

void *WholeSlideImageReader::readDataFromImage(int64_t x, int64_t y, int64_t width, int64_t height, int32_t level)
{
    if(_valid)
    {
        std::shared_lock<std::shared_mutex> l(*_mutex);
        uint32_t *buf = new uint32_t[width * height];

        //qDebug() << "Reading image:";
        //qDebug() << "x: " <<  x << " y: " << y << " " << width << " " << height;

        openslide_read_region(_obj, buf, x, y, level, width, height);

        const char *err = nullptr;
        err = openslide_get_error(_obj);
        if(err)
        {
            qDebug() << err;
        }

        return buf;
    }

    return nullptr;
}

void WholeSlideImageReader::printDimensions()
{
    if(_valid)
    {
        for(int i = 0; i < getNumberOfLevels(); i++)
        {
            std::cout << "Level: " << i << " Width: " << _dimensions[i].first << " Height: " << _dimensions[i].second << std::endl;
        }
    } else
    {
        std::cout << "Invalid image" << std::endl;
    }
}

void WholeSlideImageReader::printLevelDownsample()
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
