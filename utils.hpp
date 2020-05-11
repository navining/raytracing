#pragma once
#include <cmath>

#include "Vec3.hpp"
inline double clamp(double x) { return x < 0 ? 0 : x > 1 ? 1 : x; }

inline double getRand(double min = 0.0, double max = 1.0) {
  return min + (max - min) * rand() / RAND_MAX;
}

inline Vec3 getRandVec(double min = 0.0, double max = 1.0) {
  return Vec3(getRand(min, max), getRand(min, max), getRand(min, max));
}

Vec3 getRandVecInSphere() {
  while (true) {
    Vec3 v = getRandVec(-1.0, 1.0);
    if (v.length() < 1.0) return v;
  }
}

inline double dtog(double deg) { return M_PI * deg / 180; }