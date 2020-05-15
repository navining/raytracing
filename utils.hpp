/**
 * @file utils.hpp
 * Useful functions used in ray tracer
 * @author Navi Ning <xning5@illinois.edu>
 */
#pragma once
#include <cmath>

#include "Vec3.hpp"
/**
 * Clamp a value within 0 an 1
 * @param x value to be clamped
 * @return clamped value
 */
inline double clamp(double x) { return x < 0 ? 0 : x > 1 ? 1 : x; }

/**
 * Get a random double from min to max
 * @param min min value
 * @param max max value
 * @return a random double from min to max
 */
inline double getRand(double min = 0.0, double max = 1.0) {
  return min + (max - min) * rand() / RAND_MAX;
}

/**
 * Get a random point from min to max
 * @param min min value
 * @param max max value
 * @return a random point from min to max
 */
inline Vec3 getRandVec(double min = 0.0, double max = 1.0) {
  return Vec3(getRand(min, max), getRand(min, max), getRand(min, max));
}

/**
 * Get a random point within a sphere
 * @return a random point within a sphere
 */
Vec3 getRandVecInSphere() {
  while (true) {
    Vec3 v = getRandVec(-1.0, 1.0);
    if (v.length() < 1.0) return v;
  }
}

/**
 * Change degree to gradian
 * @param deg degree
 * @return gradian
 */
inline double dtog(double deg) { return M_PI * deg / 180; }