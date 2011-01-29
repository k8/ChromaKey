#include "image.h"
#include "imagesprocessor.h"

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
        this->file = file;
    }
}

Image::Image(const Size& size, const QRgb& color)
{
    image = Mat::ones(size, CV_8UC3);
    ImagesProcessor::fill(image, color);
    opened = false;
}

Image::Image(const Image &other)
    : QObject()
{
    image = other.image;
    opened = other.opened;
    file = other.file;
}

const Mat& Image::get(bool)
{
    return image;
}

QImage Image::getResizedQImage(const QSize &size)
{
    Mat tmp;
    resize(image, tmp, Size(size.width(), size.height()));
    return ImagesProcessor::fromCvMat(tmp);
}

ImageSaver::ImageSaver(const QString &file)
    : file(file)
{
}

bool ImageSaver::save(const Mat &image)
{
    return imwrite(file.toStdString(), image);
}
