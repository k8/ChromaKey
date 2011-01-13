#include "imagefactory.h"

ImageFactory::ImageFactory()
{
}

Image* ImageFactory::createImage(const QString &file)
{
    if (file.contains(".avi") or file.contains(".AVI"))
    {
        return new Movie(file);
    }
    else
    {
        return new Image(file);
    }
}

ImageSaver* ImageFactory::createSaver(const QString &file, Image *fg, Image *bg, const Mat &img)
{
    if (fg->isMovie())
    {
        return new MovieSaver(file, static_cast<Movie*>(fg), img.size());
    }
    if (bg->isMovie())
    {
        return new MovieSaver(file, static_cast<Movie*>(bg), img.size());
    }
    return new ImageSaver(file);
}
