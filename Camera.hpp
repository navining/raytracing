#pragma once
#include "Ray.hpp"
#include "Vec3.hpp"
#include "utils.hpp"

class Camera {
 public:
  Camera(Vec3 eyept, Vec3 lookat, Vec3 up, double vfov, double aspect_ratio) {
    origin = eyept;
    double theta = dtog(vfov);
    double half_h = tan(theta / 2);
    double half_w = aspect_ratio * half_h;

    Vec3 w = (eyept - lookat).normalize();
    Vec3 u = up.cross(w).normalize();
    Vec3 v = w.cross(u);

    horizontal = 2 * half_w * u;
    vertical = 2 * half_h * v;
    lower_left_corner = eyept - half_w * u - half_h * v - w;
  }

  Ray getRay(double u, double v) {
    return Ray(origin,
               lower_left_corner + u * horizontal + v * vertical - origin);
  }

 private:
  Vec3 origin;
  Vec3 horizontal;
  Vec3 vertical;
  Vec3 lower_left_corner;
};