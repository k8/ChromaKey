#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "image.h"
#include "movie.h"

class ImageFactory
{
public:
    ImageFactory();
    static Image* create(const QString& file);
};

#endif // IMAGEFACTORY_H
