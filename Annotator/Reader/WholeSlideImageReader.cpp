//
// Created by Daniel Pietsch on 03.08.22.
//

#include "WholeSlideImageReader.h"
#include <QDebug>

WholeSlideImageReader::WholeSlideImageReader(std::string extension) : ImageReader(extension) {

    _obj = nullptr;
    _bg_r = 255;
    _bg_g = 255;
    _bg_b = 255;

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
        _valid = false;
        qDebug() << "Unable to open Image";
        return;
    }

    if(const char *error = openslide_get_error(_obj))
    {
        _errorState = error;
        _valid = false;
        std::cout << _errorState << std::endl;
        return;
    }

    if(_errorState.empty()) {
        int discardedLevels = 0;
        for (int i = 0; i < openslide_get_level_count(_obj); i++)
        {
            int64_t x, y = 0;
            openslide_get_level_dimensions(_obj, i, &x, &y);
            if(x >= 1000 || y >= 1000)
            {
                std::pair<int, int> dims = std::make_pair(x, y);
                _dimensions.push_back(dims);
                _scaleFactors.push_back(openslide_get_level_downsample(_obj, i));
            } else
            {
                discardedLevels++;
            }
        }

        _numberOfLevels = openslide_get_level_count(_obj) - discardedLevels;

        if(_numberOfLevels == 0)
        {
            //if first levels are all very small, the view will be very pixeled, so just discard those levels and only load those, which have a proper size
            _valid = false;
            return;
        }

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

        if(const char *value = openslide_get_property_value(_obj, OPENSLIDE_PROPERTY_NAME_BACKGROUND_COLOR))
        {
            _propertiesMap.insert(std::make_pair("BackgroundColor", std::string(value)));
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

    //printLevelDownsample();
    //printDimensions();
    //printProperties();

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


unsigned char *WholeSlideImageReader::readDataFromImage(int64_t x, int64_t y, int64_t width, int64_t height, int32_t level)
{
    if(_valid)
    {
        std::shared_lock<std::shared_mutex> l(*_mutex);
        uint32_t *buf = new  uint32_t [width * height * 4];

        //qDebug() << "Reading image:";
        //qDebug() << "x: " <<  x << " y: " << y << " " << width << " " << height << " Level: " << level;

        openslide_read_region(_obj, buf, x, y, level, width, height);

        const char *err = nullptr;
        err = openslide_get_error(_obj);
        if(err)
        {
            qDebug() << err;
        }

        //OpenSlide returns buffer in form BGRA, we have to reverse it and strip the alpha channel. Source: https://openslide.org/docs/premultiplied-argb/
        unsigned char *rgb = new unsigned char[width * height * 3];
        unsigned char* bgra = (unsigned char*)buf;
        for (unsigned long long i = 0, j = 0; i < width*height*4; i+=4, j+=3) {
            if (bgra[i + 3] == 255)
            {
                rgb[j] = bgra[i + 2];
                rgb[j + 1] = bgra[i + 1];
                rgb[j + 2] = bgra[i];
            }
            else if (bgra[i + 3] == 0)
            {
                rgb[j] = _bg_r;
                rgb[j + 1] = _bg_g;
                rgb[j + 2] = _bg_b;
            }
            else
            {
                rgb[j] = (255. * bgra[i + 2]) / bgra[i + 3];
                rgb[j + 1] = (255. * bgra[i + 1]) / bgra[i + 3];
                rgb[j + 2] = (255. * bgra[i]) / bgra[i + 3];
            }
        }
        delete[] buf;
        return rgb;
    }

    return nullptr;
}
