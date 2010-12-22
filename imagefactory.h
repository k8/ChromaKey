#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "image.h"
#include "movie.h"

class ImageFactory
{
public:
    ImageFactory();
    static Image* createImage(const QString& file);
    static ImageSaver* createSaver(const QString& file, Image* fg, Image* bg, const Mat& img);
};

#endif // IMAGEFACTORY_H
