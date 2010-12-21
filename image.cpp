#include "image.h"
#include "imagesprocessor.h"


Image::Image(const Size& size, const QRgb& color)
{
    image = Mat::ones(size, CV_8UC3);
    ImagesProcessor::fill(image, color);
    opened = false;
}

QImage Image::getResizedQImage(const QSize &size)
{
    Mat tmp;
    resize(image, tmp, Size(size.width(), size.height()));
    return ImagesProcessor::fromCvMat(tmp);
}

Image::Image(const QString &file)
{
    opened = true;
    Mat tmp = imread(file.toStdString());
    if (tmp.data == NULL)
    {
        opened = false;
    }
    else
    {
        image = tmp;
    }
}

const Mat& Image::get(bool)
{
    return image;
}

