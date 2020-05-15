/**
 * @file Ray.hpp
 * The Ray class
 * @author Navi Ning <xning5@illinois.edu>
 */

#pragma once
#include "Vec3.hpp"

/**
 * Class Ray
 */
class Ray {
 public:
  Ray(Vec3 _orig, Vec3 _dir) : orig(_orig), dir(_dir) {}

  /**
   * The point at t
   * @param t lerp parameter
   * @return the point at t
   */
  Vec3 at(double t) const { return orig + t * dir; }

  Vec3 orig;  // Origin
  Vec3 dir;   // Direction
};