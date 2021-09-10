#include "Star.h"

Star::Star() {
  m = 0;
  r.fill(0);
  v.fill(0);
  a.fill(0);
}

Star::Star(double m, std::array<double, 3> r, std::array<double, 3> v) {
  this->m = m;
  this->r = r;
  this->v = v;
}



