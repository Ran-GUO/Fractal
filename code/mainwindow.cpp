
#include "mainwindow.h"
#include "mandelbrotimage.h"
#include "elec4_util.h"
#include "splineimage.h"
//#include "dynamique.h"
#include <iostream>
#include <string>
#include <vector>
#include <QtWidgets>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <thread>
#include <QKeyEvent>
#include <QWidget>
#include <QTimer>
#include <QFrame>


int max_threads=4;
double d=1; double xc=-0.5; double yc=0; int N=512;
// double d=0.0002; double xc=-1.7685736562992577; -0.000964296850972570525; int N=512;
// double d=10e-15; double xc=-1.7685736562992577; -0.000964296850972570525; int N=512;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  //
  // construct image_widget

  image_widget_ = new QLabel;
  image_widget_->setBackgroundRole(QPalette::Base);
  image_widget_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  image_widget_->setScaledContents(true);
  image_widget_->setMinimumSize(600, 400);

  create_actions();
  create_menus();

  setCentralWidget(image_widget_);
  resize(QGuiApplication::primaryScreen()->availableSize() * 2 / 5);

}


void MainWindow::create_actions() {
  //
  action_checker_board_image_ = new QAction(tr("Checker Board Image"), this);
  connect(action_checker_board_image_, SIGNAL(triggered()), this, SLOT(slot_load_checker_board_image()));

  action_exit_ = new QAction(tr("E&xit"), this);
  action_exit_->setShortcut(tr("Ctrl+Q"));
  connect(action_exit_, SIGNAL(triggered()), this, SLOT(slot_exit()));

  action_about_ = new QAction(tr("&About"), this);
  connect(action_about_, SIGNAL(triggered()), this, SLOT(slot_about()));
}

void MainWindow::slot_exit() {
  close();
}

void MainWindow::slot_about() {
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The user can select several images to be displayed on the screen</p>"));
}


void MainWindow::create_menus() {
  menu_open_ = new QMenu(tr("&Open"), this);

  menu_open_->addAction(action_checker_board_image_);
  menu_open_->addAction(action_exit_);


  menu_help_ = new QMenu(tr("&Help"), this);
  menu_help_->addAction(action_about_);

  menuBar()->addMenu(menu_open_);
  menuBar()->addMenu(menu_help_);
}



//
// Checker Board
//
const int checker_board_width = 600;
const int checker_board_height = 400;
int n[Pwidth][Pheight]={{0}};
double zn[Pwidth][Pheight]={{0}};//ici zn est z carrer

void calculer(int n_[][Pheight],double zn_[][Pheight],int w,int i,MandelbrotImage Mandelbrot,int flag){

    // iterate on each point of the image
     for (int yp = 0; yp <Pheight; ++yp) {
     // convert vertical pixel domain into real y domain
         double y0 = Mandelbrot.v_pixel2rect (yp);
         for (int xp = w; xp < w+Pwidth/max_threads; ++xp) {
         // convert horizontal pixel domain into real x domain
             double x0 = Mandelbrot.h_pixel2rect (xp);
             int n0=Mandelbrot.getNmax();
        std::complex<double> z;
        std::complex<double> c0;
        if (flag==0){
        z= std::complex<double> (0,0);
        c0=std::complex<double> (x0, y0);
        }
        else{
        z=std::complex<double> (x0,y0);
        c0=std::complex<double> (-0.4, 0.6);
        }
        //draw
        while(n0--){
           z=std::pow(z,2)+c0;
           if(std::abs(z)>=256)
               break;
        }
      n_[xp-i*Pwidth/max_threads][yp]=n0;
      zn_[xp-i*Pwidth/max_threads][yp]=std::abs(z)*std::abs(z);

    }
    }
}


void process_sub_image(int i,int m,MandelbrotImage Mandelbrot,int flag){
    int n_[(Pwidth/m)][Pheight];
    double zn_[Pwidth/m][Pheight];

    calculer(n_,zn_,i*Pwidth/m,i,Mandelbrot,flag);
    for(int p=0;p<Pheight;p++)
        for(int q=0;q<Pwidth/m;q++){
            n[q+i*Pwidth/m][p]=n_[q][p];
            zn[q+i*Pwidth/m][p]=zn_[q][p];
      }
}

void calculer_optimised(int n_[][Pheight],double zn_[][Pheight],int w,int i,MandelbrotImage Mandelbrot,int flag){

    // iterate on each point of the image
     for (int yp = 0; yp <Pheight; ++yp) {
     // convert vertical pixel domain into real y domain
         double y0 = Mandelbrot.v_pixel2rect (yp);
         for (int xp = w; xp < w+Pwidth/max_threads; ++xp) {
         // convert horizontal pixel domain into real x domain
             double x0 = Mandelbrot.h_pixel2rect (xp);
             int n0=Mandelbrot.getNmax();
             std::complex<double> z;
             std::complex<double> c0;
           if (flag==0){
             z=std::complex<double> (0,0);
             c0=std::complex<double> (x0, y0);
             }
           else{
             z=std::complex<double> (x0,y0);
             c0=std::complex<double> (-0.4, 0.6);
             }
        //draw
        while(n0--){
           z=std::pow(z,2)+c0;
           if(std::abs(z)>=256)
               break;
        }
      n_[xp-i*Pwidth/max_threads][yp]=n0;
      zn_[xp-i*Pwidth/max_threads][yp]=std::norm(z);//optimised ici par std::norm

    }
    }
}

void process_sub_image_optimised(int i,int m,MandelbrotImage Mandelbrot,int flag){
    int n_[(Pwidth/m)][Pheight];
    double zn_[Pwidth/m][Pheight];

    calculer_optimised(n_,zn_,i*Pwidth/m,i,Mandelbrot,flag);
    for(int p=0;p<Pheight;p++)
        for(int q=0;q<Pwidth/m;q++){
            n[q+i*Pwidth/m][p]=n_[q][p];
            zn[q+i*Pwidth/m][p]=zn_[q][p];
      }
}
//
//get couleur tableau [2048]
//
double sp1[2048]={0};
double sp2[2048]={0};
double sp3[2048]={0};

void get_couleur(double* sp1,double* sp2,double* sp3){
//spline
std::vector<double> px{   0.,   0.16,   0.42,   0.6425,  0.8575};
std::vector<double> pyr{  0.,    32.,   237.,     215.,      0.};
std::vector<double> pyg{  7.,   107.,   255.,     170.,     10.};
std::vector<double> pyb{ 100., 183. , 235.  ,  40.    , 15.    };

SplineImage spline1(px, pyr);
SplineImage spline2(px, pyg);
SplineImage spline3(px, pyb);

//sp1 red
double x=-0.05;
for (int i=0; i<2048; i++){
    if (spline1.get_value(x)>255){
        sp1[i]=255;
    }
    else if (spline1.get_value(x)<0){
        sp1[i]=0;
    }
    else {
        sp1[i]=spline1.get_value(x);
    }
    x=x+(1.10/2048);
}
//sp2 green
x=-0.05;
for (int i=0; i<2048; i++){
    if (spline2.get_value(x)>255){
        sp2[i]=255;
    }
    else if (spline2.get_value(x)<0){
        sp2[i]=0;
    }
    else {
        sp2[i]=spline2.get_value(x);
    }
    x=x+(1.10/2048);
}
//sp3 blue
x=-0.05;
for (int i=0; i<2048; i++){
    if (spline3.get_value(x)>255){
        sp3[i]=255;
    }
    else if (spline3.get_value(x)<0){
         sp3[i]=0;
    }
    else {
        sp3[i]=spline3.get_value(x);
    }
    x=x+(1.10/2048);
}
}
/*
void MainWindow::paintEvent(QPaintEvent *){

}*/

void MainWindow::keyPressEvent(QKeyEvent *event){

    setFocusPolicy(Qt::StrongFocus);
    installEventFilter(this);
    this->setFocus();
    this->grabKeyboard();
    std::cout<<"keyBoard"<<std::endl;
    switch (event->key()) {
      case Qt::Key_Left:
          xc=xc-0.5;
         std::cout<<"left: xc="<<xc<<std::endl;
         std::cout<<"cliquez open->check board image,s'il vous plait"<<std::endl;
          break;
      case Qt::Key_Right:
          xc=xc+0.5;
          std::cout<<"right: xc="<<xc<<std::endl;
          std::cout<<"cliquez open->check board image,s'il vous plait"<<std::endl;
          break;
      case Qt::Key_Down:
          yc=yc-0.5;
          std::cout<<"down: yc="<<yc<<std::endl;
          std::cout<<"cliquez open->check board image,s'il vous plait"<<std::endl;
          break;
      case Qt::Key_Up:
          yc=yc+0.5;
          std::cout<<"up: yc="<<yc<<std::endl;
          std::cout<<"cliquez open->check board image,s'il vous plait"<<std::endl;
          break;
      case Qt::Key_Plus:
          d=d*0.8;
          std::cout<<"+ : d="<<d<<std::endl;
          std::cout<<"cliquez open->check board image,s'il vous plait"<<std::endl;
          break;
      case Qt::Key_Minus:
          d=d*1.2;
          std::cout<<"- : d="<<d<<std::endl;
          std::cout<<"cliquez open->check board image,s'il vous plait"<<std::endl;
          break;
      case Qt::Key_T:
          if(flag==1) {
              flag=0;
               std::cout<<"T: le passage a une fractale Mandelbrot"<<std::endl;
          }
          else {
              flag=1;
              std::cout<<"T: le passage a une fractale Julia"<<std::endl;
          }
          std::cout<<"cliquez open->check board image,s'il vous plait"<<std::endl;
          break;
      default:
            break;
      }
    update();
   // QWidget::keyPressEvent(event);
}
/*
void MainWindow::keyReleaseEvent(QKeyEvent *)
{

}*/




void MainWindow::slot_load_checker_board_image() {

  // The ~10 lines of code below must be changed with your code 
  // Create a QImage of required size
  // Draw a simple black/white checker board

     std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();


  QImage checker_board_image(checker_board_width, checker_board_height, QImage::Format_RGB32);
  QPainter painter(&checker_board_image);
  painter.fillRect(checker_board_image.rect(), Qt::white);

     get_couleur(sp1,sp2,sp3);
     MandelbrotImage Mandelbrot(2*d,3*d,xc,yc,N);
    std::vector<std::thread> threads;

       for(int i=0;i<max_threads;i++){
           threads.emplace_back([=](){
         //process_sub_image_optimised(i,max_threads,Mandelbrot);
         process_sub_image(i,max_threads,Mandelbrot,flag);
           });
        }




       for (auto &thread_elem:threads){
           thread_elem.join();
       }

            for (int yp = 0; yp < Pheight; ++yp) {
                for (int xp = 0; xp < Pwidth; ++xp) {

           if(n[xp][yp]<0){
               painter.setPen(QPen(QColor(0,0,0)));
               painter.drawPoint(xp,yp);
           }
           else{
               double v=log(log(zn[xp][yp])/log(2))/log(2);  //log2(x)=ln(x)/ln(2), log=ln
               int i=1024*sqrt((Mandelbrot.getNmax()-n[xp][yp])+5-v);
               if(i>=2048) i=i%2048;

                painter.setPen(QPen(QColor(sp1[i], sp2[i] , sp3[i])));
                painter.drawPoint(xp,yp);
               //checker_board_image.setPixelColor(xp,yp,qRgb(sp1[i], sp2[i] , sp3[i] ));
           }

        }
    }


   std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
   std::chrono::duration<double> elapsed_time = end - start;
   std::cout << std::fixed << std::setprecision(0)<< "Info: image calculated in " << ELEC4::Commify((long long int)(elapsed_time.count() * 10e6)) <<"us" << std::endl;

  //
  // You probably don't want to change these lines 
  //
   image_widget_->setPixmap(QPixmap::fromImage(checker_board_image));
   image_widget_->setFixedSize(checker_board_width, checker_board_height);
   adjustSize();

}

