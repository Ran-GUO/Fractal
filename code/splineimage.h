#ifndef SPLINEIMAGE_H
#define SPLINEIMAGE_H

#include <QObject>
#include <QImage>
#include <Eigen/Dense>
#include <vector>


class SplineImage : public QImage
{
private:
    std::vector<double> xs_;
    std::vector<double> ys_;
    std::vector<double> as_;
    std::vector<double> bs_;
    std::vector<double> cs_;
    std::vector<double> ds_;
    size_t size_;
public:
    SplineImage(std::vector<double> xs,std::vector<double> ys) : xs_(xs), ys_(ys), ds_(ys){
        size_ = xs.size();
        //M*u=v;
        Eigen::MatrixXd M(3 * size_, 3 * size_);
        Eigen::VectorXd v(3 * size_);
        //initialize
        M.setZero();
        v.setZero();

        double xN=1.0;
        xs_.push_back(xN);
        ys_.push_back(ys_[0]);

        for(size_t i=0;i<size_-1;++i){
          double delta=xs_[i+1]-xs_[i];
          //on goupe par 3 lignes. 3 lignes=1 groupe i.
                //premier ligne de chaque groupe:
                //ligne 3*i+0
                //ai*delta^3+bi*delta^2+ci*delta=y(i+1)-yi.
                M(3*i,3*i+0)= delta*delta*delta;
                M(3*i,3*i+1)= delta*delta;
                M(3*i,3*i+2)= delta;
                v(3*i)=ys_[i+1]-ys[i];

                //2eme ligne de chaque groupe: first derivative=0.
                //ligne 3*i+1
                //3*delta^2*ai+2*delta*bi+ci=c[i+1]
                M(3*i+1,3*i)  = 3*delta*delta;
                M(3*i+1,3*i+1)= 2*delta;
                M(3*i+1,3*i+2)= 1.0;
                M(3*i+1,3*i+3+2)=-1.0; //-c[i+1]

                //3eme ligne de chaque groupe: second derivative=0.
                //ligne 3*i+2
                //6*delta*ai+2*bi=2*b[i+1]
                M(3*i+2,3*i)  = 6*delta;
                M(3*i+2,3*i+1)= 2.0;
                M(3*i+2,3*i+3+1)= -2.0; //-2*b[i+1]
              }

              //last groupe when i=size_-1
              //conditions limites
              //1er ligne:
               size_t i=size_-1;
               double delta=xs_[i+1]-xs_[i];
               M(3*i,3*i+0)= delta*delta*delta;
               M(3*i,3*i+1)= delta*delta;
               M(3*i,3*i+2)= delta;
               v(3*i)=ys_[i+1]-ys[i]; //on supose ys_[N]=ys_[0]

              //2eme ligne
              //R0'(x0)=0; on suppose x0=0:
              //c0=0
              M(3*i+1,2)=1.0;

              //3eme ligne
              //RN-1'(xN)=0:
              //3*delta^2*ai[N-1]+2*delta*b[N-1]+c[N-1]=0
              M(3*i+2,3*i)  = 3*delta*delta;
              M(3*i+2,3*i+1)= 2*delta;
              M(3*i+2,3*i+2)= 1.0;

              // Solve M.u=v => u = M^(-1).v
              Eigen::VectorXd u(3 * size_);
              u=M.colPivHouseholderQr().solve(v);

                  for (size_t i = 0; i < size_; ++i) {
                    as_.push_back(u[3*i + 0]);
                    bs_.push_back(u[3*i + 1]);
                    cs_.push_back(u[3*i + 2]);
                }
        }

     double get_value(const double x) const;

};

#endif // SPLINEIMAGE_H
