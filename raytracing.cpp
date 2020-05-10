/**
 * @file raytracing.cpp
 * A simple ray tracer
 * @author Navi Ning <xning5@illinois.edu>
 */

#include <cmath>
#include <iostream>

#include "Ray.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"
#define WIDTH 960
#define HEIGHT 720
#define FILENAME "result.ppm"
#define SPP 10

#define RED Vec3(1, 0, 0)
#define BLUE Vec3(0, 0, 1)
#define WHITE Vec3(1, 1, 1)

inline double clamp(double x) { return x < 0 ? 0 : x > 1 ? 1 : x; }

inline double getRand(double min = 0.0, double max = 1.0) {
  return min + (max - min) * rand() / RAND_MAX;
}

// --------------- Scene -------------------
// Camera
Vec3 origin(0.0, 0.0, 0.0);
Vec3 horizontal(4.0, 0.0, 0.0);
Vec3 vertical(0.0, 3.0, 0.0);
Vec3 lower_left_corner =
    origin - horizontal * 0.5 - vertical * 0.5 - Vec3(0, 0, 1);

// Spheres
Sphere spheres[] = {
    Sphere(Vec3(0, -100.5, -1), 100, BLUE),
    Sphere(Vec3(0, 0, -1), 0.5, RED),
};
// -----------------------------------------

Vec3 rayColor(const Ray &r) {
  bool isHit = false;
  double t = INFINITY;
  Sphere s;
  for (Sphere &cur : spheres) {
    double dis = cur.hit(r);
    if (dis > 0.0) {
      isHit = true;
      if (dis < t) {
        t = dis;
        s = cur;
      }
    }
  }

  if (isHit) {
    // Ray hits an object
    // Normal
    Vec3 N = r.at(t) - s.loc;
    N = N.normalize();
    // Front or back
    bool isFront = r.dir.dot(N) > 0 ? false : true;
    return 0.5 * Vec3(N.x + 1, N.y + 1, N.z + 1);
  }

  Vec3 unit = r.dir.normalize();
  t = 0.5 * (unit.y + 1.0);
  return (1.0 - t) * WHITE + t * Vec3(0.5, 0.7, 1.0);
}

int main() {
  const int w = WIDTH;
  const int h = HEIGHT;
  Vec3 *image = new Vec3[w * h]();

  std::cout << "Target image: " << w << " × " << h << std::endl;
  std::cout << "Start rendering..." << std::endl;

#pragma omp parallel for schedule(dynamic, 1)
  for (int j = 0; j < h; j++) {
    // Loop through rows
    if (j * 20 % HEIGHT == 0)
      std::cerr << "\rProgress: " << (int)100.0 * j / (h - 1) << "%\n"
                << std::flush;
    for (int i = 0; i < w; i++) {
      // Loop through columns
      Vec3 color;
      for (int s = 0; s < SPP; s++) {
        // Loops through multiple samples
        double u = (i + getRand()) / (w - 1);
        double v = (j + getRand()) / (h - 1);
        Ray r(origin, lower_left_corner + u * horizontal + v * vertical);
        color += rayColor(r);
      }
      image[(h - j - 1) * w + i] = color / SPP;
    }
  }
  // Write ppm image
  FILE *f = fopen(FILENAME, "w");
  fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
  for (int i = 0; i < w * h; i++) {
    fprintf(f, "%d %d %d ", (int)(255.999 * image[i].x),
            (int)(255.999 * image[i].y), (int)(255.999 * image[i].z));
  }

  std::cout << "Finish!" << std::endl;
  std::cout << "The image is saved as " << FILENAME << "." << std::endl;
  delete[] image;
  return 0;
}