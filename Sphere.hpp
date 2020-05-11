#pragma once
#include "Ray.hpp"
#include "Vec3.hpp"
class Sphere {
 public:
  Sphere() {}
  Sphere(Vec3 _loc, double _rad, Vec3 _color, int _mat)
      : rad(_rad), loc(_loc), color(_color), mat(_mat) {}

  /**
   * Return distance to the hit point, -1.0 if not hit
   */
  double hit(const Ray& r) {
    Vec3 ol = r.orig - loc;
    double a = r.dir.dot(r.dir);
    double b = 2.0 * ol.dot(r.dir);
    double c = ol.dot(ol) - rad * rad;
    double dis = b * b - 4 * a * c;
    if (dis < 0) {
      return -1.0;
    } else {
      double t = (-b - sqrt(dis)) / (2.0 * a);
      // Fixing shadow acne
      double eps = 0.001;
      return t < eps && t > -eps ? 0.0 : t;
    }
  }

  double rad;  // Radius
  Vec3 loc;    // Location
  Vec3 color;  // Color
  int mat;     // Material
};