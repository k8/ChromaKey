#ifndef PICTURE_H
#define PICTURE_H

#include "image.h"

class Picture : public Image
{
public:
    Picture();
    bool open(const QString& file);
    const Mat& get();
    QImage getResizedQImage(const QSize& size);
};

#endif // PICTURE_H
