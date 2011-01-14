#include "anglekeyer.h"

#include <QDebug>
#include <opencv/highgui.h>

AngleKeyer::AngleKeyer()
{
}

double abs(double x)
{
    if (x < 0)
        x = -x;
    return x;
}

void AngleKeyer::computeMatte()
{
    double alpha = (kp->getAlpha()/180.0)*M_PI;
    Color col(kp->getColor().rgb());
    Mat fgYCrCb;
    cvtColor(fg, fgYCrCb, CV_BGR2YCrCb);
    bg.copyTo(comp);
    Rect r(comp.cols-fg.cols, comp.rows-fg.rows, fg.cols, fg.rows);
    Mat roi(comp, r);
    Mat bgYCrCb;
    cvtColor(roi, bgYCrCb, CV_BGR2YCrCb);

    int y = col.Y();
    int cr = col.Cr();
    int cb = col.Cb();

    YCbCrCordinate key_cord(cb, cr);
    double sin_tau = key_cord.sin();
    double cos_tau = key_cord.cos();

    double tan_alpha = tan(alpha/2.0);
    if (tan_alpha == 0)
        tan_alpha = 0.000001;

    double min =  0;

    int count = 0;
    for (int i=0; i<fg.rows; i++)
    {
        for (int j=0; j<fg.cols; j++)
        {
            Vec3b& elem = fgYCrCb.at<Vec3b>(i,j);
            Vec3b& bgElem = bgYCrCb.at<Vec3b>(i,j);
            int ey = elem[0];
            int ecr = elem[1];
            int ecb = elem[2];

            double ecb_norm = YCbCrCordinate::normalized(ecb);
            double ecr_norm = YCbCrCordinate::normalized(ecr);

            double X = ecb_norm*cos_tau+ecr_norm*sin_tau;
            double Z = ecr_norm*cos_tau-ecb_norm*sin_tau;

            double K_FG = 0;
            min =  abs(Z)/tan_alpha;            
            if (X > min)
            {
                K_FG = X-min;
            }
            double CB_FG = ecb-K_FG*cos_tau;
            double CR_FG = ecr-K_FG*sin_tau;

            if (K_FG > 0)
            {
                bgElem = elem;
                bgElem[0] = bgElem[0]*(1-K_FG)+ey*K_FG;
                bgElem[1] = CR_FG;
                bgElem[2] = CB_FG;
                double Y_norm = YCbCrCordinate::normalized(ey);
//                bgElem[0] = Y_norm-kp->getLuminance()*K_FG;
//                bgElem[1] = bgElem[1]*(1-Cr_FG)+ecr*Cr_FG;
//                bgElem[2] = bgElem[2]*(1-Cb_FG)+ecb*Cb_FG;
                count++;
            }
            else
            {
            }
        }
    }
    cvtColor(bgYCrCb, roi, CV_YCrCb2BGR);
}

void AngleKeyer::prepareOutput(Mat &out)
{
    if (kp->isMatteVisible())
    {
        out = matte->getMat();
    }
    else
    {
        comp.copyTo(out);
    }
}
