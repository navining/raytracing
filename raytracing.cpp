/**
 * @file raytracing.cpp
 * A simple ray tracer
 * @author Navi Ning <xning5@illinois.edu>
 */

#include <cmath>
#include <iostream>
#include <vector>

#include "Camera.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"
#include "utils.hpp"
#define WIDTH 960
#define HEIGHT 480
#define FILENAME "result.ppm"
#define SPP 5000
#define MAX_DEPTH 20

#define WHITE Vec3(1, 1, 1)
#define GRAY Vec3(0.5, 0.5, 0.5)
#define BLACK Vec3()

// Material types
enum materials { DIFFUSE, METAL, DIELECTRICS, LIGHT };

// --------------- Scene -------------------
// Camera
Vec3 eyept(2, 1, 10);   // Eye point
Vec3 lookat(2, 1, -1);  // Lookat vector
Vec3 up(0, 1, 0);       // Up vertor
Camera cam(eyept, lookat, up, 20, 1.0 * WIDTH / HEIGHT, 0.1, 10.0);

// Spheres
std::vector<Sphere> spheres = {
    Sphere(Vec3(0, -1e9, 0), 1e9, GRAY, DIFFUSE),           // Bottom
    Sphere(Vec3(0, 1e9 + 20, 0), 1e9, BLACK, LIGHT),        // Top
    Sphere(Vec3(-1e9 - 0.5, 0, 0), 1e9, GRAY, DIFFUSE),     // Left
    Sphere(Vec3(1e9 + 40, 0, 0), 1e9, BLACK, LIGHT),        // Right
    Sphere(Vec3(0, 0, -1e9 - 20), 1e9, BLACK, LIGHT),       // Front
    Sphere(Vec3(0, 0, 1e9 + 20), 1e9, BLACK, LIGHT),        // Back
    Sphere(Vec3(-30.99 + 0.5, 1.5, -1), 30, WHITE, LIGHT),  // Light
    Sphere(Vec3(0.5, 0.3, 0), 0.3, Vec3(0.2, 0.4, 0), DIFFUSE),
    Sphere(Vec3(1.7, 0.6, 0), 0.6, WHITE, DIELECTRICS),
    Sphere(Vec3(3.75, 1.2, 0), 1.2, Vec3(0.5, 0.5, 0.3), METAL),
};

/**
 * Set up the scene with random spheres
 * @param num number of spheres
 */
void setupScene(int num) {
  for (int i = 0; i < num; i++) {
    double z = getRand(-10, 10);
    while (-1.3 < z && z < 1.3) {
      z = getRand(-10, 10);
    }
    Vec3 location(getRand(-0.3, 5), 0.1, z);
    Vec3 color(getRand(), getRand(), getRand());
    double rand = getRand();
    if (rand < 0.6) {
      spheres.push_back(Sphere(location, 0.1, color, DIFFUSE));
    } else if (rand < 0.8) {
      spheres.push_back(Sphere(location, 0.1, color, METAL));
    } else {
      spheres.push_back(Sphere(location, 0.1, WHITE, DIELECTRICS));
    }
  }
}
// -----------------------------------------

/**
 * Get the pixel color with a ray
 * @param r the ray
 * @param depth recurtion depth
 * @return the color
 */
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
    } else if (s.mat == LIGHT) {
      // ----------------- LIGHT ---------------------------
      double luminance = 30.0;
      return s.color * luminance;
    }
  }

  Vec3 unit = r.dir.normalize();
  t = 0.5 * (unit.y) + 0.1;
  return t * Vec3(0, 0, 0.01);
}

/**
 * Main function
 */
int main(int argc, char *argv[]) {
  const int w = WIDTH;
  const int h = HEIGHT;
  int spp = argc == 2 ? atoi(argv[1]) : SPP;

  Vec3 *image = new Vec3[w * h]();

  std::cout << "Target image: " << w << " × " << h << std::endl;
  std::cout << "Samples per pixel: " << spp << std::endl;
  std::cout << "Start rendering..." << std::endl;

  setupScene(40);

  for (int j = 0; j < h; j++) {
    // Loop through rows
    fprintf(stderr, "\rProgress: %5.2f%%", 100. * j / (h - 1));
    for (int i = 0; i < w; i++) {
      // Loop through columns
      Vec3 color;
      for (int s = 0; s < spp; s++) {
        // Loops through multiple samples
        double u = (i + getRand()) / (w - 1);
        double v = (j + getRand()) / (h - 1);
        Ray r = cam.getRay(u, v);
        color += rayColor(r, 0);
      }
      // Gamma correction
      color = color / spp;
      color.x = sqrt(color.x);
      color.y = sqrt(color.y);
      color.z = sqrt(color.z);
      image[(h - j - 1) * w + i] = color;
    }
  }

  // Write into ppm
  FILE *f = fopen(FILENAME, "w");
  fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
  for (int i = 0; i < w * h; i++) {
    fprintf(f, "%d %d %d ", (int)(255.999 * image[i].x),
            (int)(255.999 * image[i].y), (int)(255.999 * image[i].z));
  }
  std::cout << "\nFinish!" << std::endl;
  std::cout << "The image is saved as " << FILENAME << std::endl;

  delete[] image;

  return 0;
}
