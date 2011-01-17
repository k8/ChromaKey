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

ImageSaver* ImageFactory::createSaver(const QString &file, Image *fg, Image *bg)
{
    if (file.contains(".avi") or file.contains(".AVI"))
    {
        if (fg->isMovie())
        {
            Size size = fg->getSize();
            if (bg->isMovie())
            {
                size = bg->getSize();
            }
            return new MovieSaver(file, static_cast<Movie*>(fg), size);
        }
        else
        {
            return new MovieSaver(file, static_cast<Movie*>(bg), bg->getSize());
        }
    }
    else
    {
        return new ImageSaver(file);
    }
}
