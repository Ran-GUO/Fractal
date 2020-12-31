#include "splineimage.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <iomanip>


double SplineImage::get_value(const double x) const {
  if (x < 0.0 || x > 1.0) {
     return ds_[0];//n'exist pas
  }

  for (size_t i = 0; i < xs_.size(); ++i) {
      if (x>=xs_[i] && x<=xs_[i+1]) {
      double delta = x - xs_[i];
      return as_[i]*delta*delta*delta + bs_[i]*delta*delta + cs_[i]*delta +ds_[i];
    }
  }

  return 0.0; //n'exist pas
}
