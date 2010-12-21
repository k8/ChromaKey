#include "picture.h"

Picture::Picture()
{
}

bool Picture::open(const QString &file)
{
    bool opened = true;
    Mat tmp = imread(file.toStdString());
    if (tmp.data == NULL)
    {
        opened = false;
    }
    else
    {
        image = tmp;
    }
    return opened;
}

const Mat& Picture::get()
{
    return image;
}

QImage Picture::getResizedQImage(const QSize& size)
{
}

