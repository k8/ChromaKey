#ifndef MOVIELABEL_H
#define MOVIELABEL_H

#include <QLabel>
#include <QMouseEvent>

class MovieLabel : public QLabel
{
    Q_OBJECT
public:
    MovieLabel(QWidget* parent);

signals:
    void colorChanged(QRgb color);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
};

#endif // MOVIELABEL_H
