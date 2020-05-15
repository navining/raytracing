/**
 * @file Vec3.hpp
 * The vector class
 * @author Navi Ning <xning5@illinois.edu>
 */

#pragma once
#include <cmath>

/**
 * Class Vec3
 */
class Vec3 {
 public:
  Vec3(double _x = 0, double _y = 0, double _z = 0) : x(_x), y(_y), z(_z) {}

  Vec3 operator+(const Vec3 &other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
  }

  Vec3 operator-(const Vec3 &other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
  }

  Vec3 operator*(const Vec3 &other) const {
    return Vec3(x * other.x, y * other.y, z * other.z);
  }

  Vec3 operator*(double d) const { return Vec3(d * x, d * y, d * z); }

  Vec3 operator/(double d) const { return Vec3(x / d, y / d, z / d); }

  Vec3 operator+(double d) const { return Vec3(x + d, y + d, z + d); }

  Vec3 operator-(double d) const { return Vec3(x - d, y - d, z - d); }

  void operator+=(const Vec3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
  }

  /**
   * Dot product of two vectors
   * @param v other vector
   * @return the dot product
   */
  double dot(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }

  /**
   * Length of the vector
   * @return length of the vector
   */
  double length() const { return sqrt(x * x + y * y + z * z); }

  /**
   * Normalize the vector
   * @return the normalized vector
   */
  Vec3 normalize() const {
    double l = length();
    return Vec3(x / l, y / l, z / l);
  }

  /**
   * Cross product of two vectors
   * @param v other vector
   * @return the cross product
   */
  Vec3 cross(const Vec3 &v) {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
  }

  double x, y, z;  // Coordinates
};

inline Vec3 operator*(double d, const Vec3 &v) { return v * d; }

inline Vec3 operator-(const Vec3 &v) { return Vec3(-v.x, -v.y, -v.z); }