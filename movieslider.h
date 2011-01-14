#ifndef MOVIESLIDER_H
#define MOVIESLIDER_H

#include <QSlider>

class MovieSlider : public QSlider
{
    Q_OBJECT
public:
    MovieSlider(QWidget* parent = 0);

public slots:
    void setValue(int value);

private slots:
    void draggingStarted();
    void draggingStopped();

private:
    bool draged;
};

#endif // MOVIESLIDER_H
