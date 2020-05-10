#pragma once
#include <cmath>
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

  double dot(const Vec3 &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  double length() const { return sqrt(x * x + y * y + z * z); }

  Vec3 normalize() const {
    double l = length();
    return Vec3(x / l, y / l, z / l);
  }

  double x, y, z;
};

inline Vec3 operator*(double d, const Vec3 &v) { return v * d; }
