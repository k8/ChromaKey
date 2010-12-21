#include "imagefactory.h"

ImageFactory::ImageFactory()
{
}

Image* ImageFactory::create(const QString &file)
{
    if (file.contains(".avi"))
    {
        return new Movie(file);
    }
    else
    {
        return new Image(file);
    }
}
