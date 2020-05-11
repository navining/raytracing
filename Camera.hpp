#pragma once
#include "Ray.hpp"
#include "Vec3.hpp"
#include "utils.hpp"

class Camera {
 public:
  Camera(Vec3 eyept, Vec3 lookat, Vec3 up, double vfov, double aspect_ratio,
         double aperture = 0.0, double dist = 1.0)
      : radius(aperture / 2) {
    origin = eyept;
    double theta = dtog(vfov);
    double half_h = tan(theta / 2);
    double half_w = aspect_ratio * half_h;

    Vec3 w = (eyept - lookat).normalize();
    Vec3 u = up.cross(w).normalize();
    Vec3 v = w.cross(u);

    horizontal = 2 * half_w * u * dist;
    vertical = 2 * half_h * v * dist;
    lower_left_corner = eyept - (half_w * u + half_h * v + w) * dist;
  }

  Ray getRay(double u, double v) {
    double offset = 0.0;
    if (radius > 1e5) {
      // Defocus blur
      Vec3 rand = radius * getRandVecInSphere();
      offset = u * rand.x + v * rand.y;
    }
    return Ray(origin + offset, lower_left_corner + u * horizontal +
                                    v * vertical - origin - offset);
  }

 private:
  Vec3 origin;
  Vec3 horizontal;
  Vec3 vertical;
  Vec3 lower_left_corner;
  double radius;
};