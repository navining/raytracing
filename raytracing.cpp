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
#define WIDTH 384
#define HEIGHT 216
#define FILENAME "result.ppm"

#define RED Vec3(1, 0, 0)
#define BLUE Vec3(0, 0, 1)
#define WHITE Vec3(1, 1, 1)

// --------------- Scene -------------------
Vec3 origin(0.0, 0.0, 0.0);
Vec3 horizontal(4.0, 0.0, 0.0);
Vec3 vertical(0.0, 2.25, 0.0);
Vec3 lower_left_corner =
    origin - horizontal * 0.5 - vertical * 0.5 - Vec3(0, 0, 1);
Sphere s(Vec3(0, 0, -1), 0.5, RED);
// -----------------------------------------

Vec3 rayColor(const Ray &r) {
  double t = s.hit(r);
  if (t > 0.0) {
    Vec3 N = r.at(t) - s.loc;
    N = N.normalize();
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

  for (int j = 0; j < h; j++) {
    std::cerr << "\rProgress: " << 100.0 * j / (h - 1) << "% " << std::flush;
    for (int i = 0; i < w; i++) {
      double u = 1.0 * i / (w - 1);
      double v = 1.0 * j / (h - 1);
      Ray r(origin, lower_left_corner + u * horizontal + v * vertical);
      image[(h - j - 1) * w + i] = rayColor(r);
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