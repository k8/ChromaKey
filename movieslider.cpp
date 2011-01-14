#include "movieslider.h"
#include <QDebug>

MovieSlider::MovieSlider(QWidget *parent)
    : QSlider(parent), draged(false)
{
    connect(this, SIGNAL(sliderPressed()), this, SLOT(draggingStarted()));
    connect(this, SIGNAL(sliderReleased()), this, SLOT(draggingStopped()));
}

void MovieSlider::draggingStarted()
{
    draged = true;
}

void MovieSlider::draggingStopped()
{
    draged = false;
}

void MovieSlider::setValue(int value)
{
    if (! draged)
    {
        QSlider::setValue(value);
    }
}

