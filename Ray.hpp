#pragma once
#include "Vec3.hpp"
class Ray {
 public:
  Ray(Vec3 _orig, Vec3 _dir) : orig(_orig), dir(_dir) {}
  Vec3 at(double t) const { return orig + t * dir; }
  Vec3 orig, dir;
};