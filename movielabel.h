#ifndef MOVIELABEL_H
#define MOVIELABEL_H

#include <QLabel>
#include <QMouseEvent>

class MovieLabel : public QLabel
{
    Q_OBJECT
public:
    MovieLabel(QWidget* parent);   
    void setPixmap(const QPixmap &);

signals:
    void colorChanged(QRgb color);

protected:
    QPixmap scaledPixmap(const QPixmap& pix);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void resizeEvent(QResizeEvent * ev);
};

#endif // MOVIELABEL_H
