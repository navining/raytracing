#include "Ray.hpp"
#include "Vec3.hpp"
class Sphere {
 public:
  Sphere() {}
  Sphere(Vec3 _loc, double _rad, Vec3 _color)
      : rad(_rad), loc(_loc), color(_color) {}

  /**
   * Return distance to the hit point, -1.0 if not hit
   */
  double hit(const Ray& r) {
    Vec3 ol = r.orig - loc;
    double a = r.dir.dot(r.dir);
    double b = 2.0 * ol.dot(r.dir);
    double c = ol.dot(ol) - rad * rad;
    double dis = b * b - 4 * a * c;
    return dis < 0 ? -1.0 : (-b - sqrt(dis)) / (2.0 * a);
  }

  double rad;  // Radius
  Vec3 loc;    // Location
  Vec3 color;  // Color
};