/**
 * @file raytracing.cpp
 * A simple ray tracer
 * @author Navi Ning <xning5@illinois.edu>
 */

#include <cmath>
#include <iostream>

#include "Camera.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"
#include "utils.hpp"
#define WIDTH 960
#define HEIGHT 720
#define FILENAME "result.ppm"
#define SPP 4
#define MAX_DEPTH 50

#define RED Vec3(1, 0, 0)
#define BLUE Vec3(0, 0, 1)
#define WHITE Vec3(1, 1, 1)
#define GRAY Vec3(0.5, 0.5, 0.5)

enum materials { DIFFUSE, METAL, DIELECTRICS };

// --------------- Scene -------------------
// Camera
Vec3 eyept(-13, 3, 6);
Vec3 lookat(0, 0, 0);
Vec3 up(0, 1, 0);
Camera cam(eyept, lookat, up, 20, 1.0 * WIDTH / HEIGHT);
// Spheres
Sphere spheres[] = {
    Sphere(Vec3(0, -1000, 0), 1000, GRAY, DIFFUSE),
    Sphere(Vec3(4, 1, 0), 1.0, Vec3(.7, .6, .5), METAL),
    Sphere(Vec3(-4, 1, 0), 1.0, Vec3(.4, .2, .1), DIFFUSE),
    Sphere(Vec3(0, 1, 0), 1.0, WHITE, DIELECTRICS),
};
// -----------------------------------------

Vec3 rayColor(const Ray &r, int depth) {
  if (depth >= MAX_DEPTH) return Vec3();

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
    // Hit point
    Vec3 p = r.at(t);
    // Outward normal
    Vec3 out_N = p - s.loc;
    out_N = out_N.normalize();
    // Front or back
    bool isFront = r.dir.dot(out_N) > 0 ? false : true;
    // Normal
    Vec3 N = isFront ? out_N : -out_N;

    if (s.mat == DIFFUSE) {
      // ------------------ Diffuse material ------------------
      Vec3 target = p + N + getRandVecInSphere();
      return s.color * rayColor(Ray(p, target - p), depth + 1);
    } else if (s.mat == METAL) {
      // ------------------ Metal material ------------------
    REFLECT:
      Vec3 V = r.dir.normalize();
      Vec3 reflected = V - 2 * V.dot(N) * N;
      if (reflected.dot(N) <= 0) return Vec3(0, 0, 0);
      // double fuzziness = s.rad < 1 ? s.rad : 1;
      double fuzziness = 0;
      Vec3 fuzz = fuzziness * getRandVecInSphere();
      return s.color * rayColor(Ray(p, reflected + fuzz), depth + 1);
    } else if (s.mat == DIELECTRICS) {
      // ------------------ Dielectrics material ------------------
      double factor = 1.52;
      Vec3 V = r.dir.normalize();
      double cos_theta = fmin((-V).dot(N), 1.0);
      double eta = isFront ? 1.0 / factor : factor;
      double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

      if (eta * sin_theta <= 1.0) {
        // Refract
        // Schlick approximation
        double r0 = (1 - eta) / (1 + eta);
        r0 *= r0;
        double reflect_prob = r0 + (1 - r0) * pow(1 - cos_theta, 5);
        if (getRand() < reflect_prob) goto REFLECT;

        Vec3 refracted_parallel = eta * (V + cos_theta * N);
        Vec3 refracted_perp =
            -sqrt(1.0 - pow(refracted_parallel.length(), 2)) * N;
        Vec3 refracted = refracted_parallel + refracted_perp;
        return rayColor(Ray(p, refracted), depth + 1);
      } else {
        // Reflect
        goto REFLECT;
      }
    }
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
  std::cout << "Samples per pixel: " << SPP << std::endl;
  std::cout << "Start rendering..." << std::endl;

  //#pragma omp parallel for schedule(dynamic, 1)
  for (int j = 0; j < h; j++) {
    // Loop through rows
    if (j * 20 % HEIGHT == 0)
      std::cerr << "\rProgress: " << (int)100.0 * j / (h - 1) << "%...\n"
                << std::flush;
    for (int i = 0; i < w; i++) {
      // Loop through columns
      Vec3 color;
      for (int s = 0; s < SPP; s++) {
        // Loops through multiple samples
        double u = (i + getRand()) / (w - 1);
        double v = (j + getRand()) / (h - 1);
        Ray r = cam.getRay(u, v);
        color += rayColor(r, 0);
      }
      // Gamma correction
      color = color / SPP;
      color.x = sqrt(color.x);
      color.y = sqrt(color.y);
      color.z = sqrt(color.z);
      image[(h - j - 1) * w + i] = color;
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