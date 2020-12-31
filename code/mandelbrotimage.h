#ifndef MANDELBROTIMAGE_H
#define MANDELBROTIMAGE_H
#include <complex>
#include <QImage>
#include <vector>
#include <chrono>
#include <sstream>
#include <string>


const int Pwidth = 600;
const int Pheight = 400;

extern std::chrono::duration<double> elapsed_time;


class MandelbrotImage : public QImage
{
private:
    double height;
    double width;
    double xc;
    double yc;
    int  n_max;

public:
    MandelbrotImage(const double h=2,const double w=3,const double x=-0.5,const double y=0,const int nmax=512):height(h),width(w),xc(x),yc(y),n_max(nmax){};

    double v_pixel2rect (const int yp){
    return ((double)Pheight-(double(yp)))/(double)Pheight *height -((height/2)-yc);
    }

    //
    double h_pixel2rect (const int xp){
    return ((double)xp)/(double)Pwidth * width -((width/2)-xc);
    }

    int getNmax(){return this->n_max;}

};



#endif // MANDELBROTIMAGE_H
