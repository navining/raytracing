/**
 * @file Camera.hpp
 * The Camera class
 * @author Navi Ning <xning5@illinois.edu>
 */

#pragma once
#include "Ray.hpp"
#include "Vec3.hpp"
#include "utils.hpp"

/**
 * Class Camera
 */
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

  /**
   * Get a ray from pixel coordinates
   * @param u pixel coordinate
   * @param v pixel coordinate
   * @return a ray through the pixel
   */
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
  Vec3 origin;             // Origin point
  Vec3 horizontal;         // Horizontal plane
  Vec3 vertical;           // Vertical plane
  Vec3 lower_left_corner;  // Lower left corner
  double radius;           // Aperture radius
};