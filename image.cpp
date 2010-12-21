#include "image.h"

Image::Image()
{
}

QImage Image::getResizedQImage(const QSize &size)
{
    Mat tmp;
    resize(image, tmp, Size(size.width(), size.height()));
    return ImagesProcessor::fromCvMat(tmp);
}
