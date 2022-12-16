//
// Created by Daniel Pietsch on 15.12.22.
//

#ifndef ANNOTATOR_IMAGEFACTORY_H
#define ANNOTATOR_IMAGEFACTORY_H
#include "ImageReader.h"


class ImageFactory {
public:
    ImageFactory();
    ~ImageFactory();
    std::shared_ptr<ImageReader> getImageTyp(std::string path);

private:
    std::map<std::string, int> _supportedExtensions
    {
            {".jpeg", 0},
            {".jpg",  0},
            {".png",  0},
            {".tif",  0}, //doesnt really work right now, maybe add tiff/tif image reader
            {".svs" , 1},
            {".ndpi", 1},
            {".scn",  1},
            {".mrxs", 1}
    };
};


#endif //ANNOTATOR_IMAGEFACTORY_H
