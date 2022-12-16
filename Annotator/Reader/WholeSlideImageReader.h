//
// Created by Daniel Pietsch on 03.08.22.
//

#ifndef ANNOTATOR_WHOLESLIDEIMAGEREADER_H
#define ANNOTATOR_WHOLESLIDEIMAGEREADER_H
#include <vector>
#include "openslide.h"
#include "ImageReader.h"
#include <string>
#include <iostream>
#include <cmath>


class WholeSlideImageReader : public ImageReader {
public:
    WholeSlideImageReader(std::string extension);
    ~WholeSlideImageReader();

private:
    openslide_t *_obj;
    int _bg_r;
    int _bg_g;
    int _bg_b;

public:
    void initializeImage(const std::string imagePath);
    void cleanUp();
    unsigned char *readDataFromImage(int64_t x, int64_t y, int64_t width, int64_t height, int32_t level);


};


#endif //ANNOTATOR_WHOLESLIDEIMAGEREADER_H
