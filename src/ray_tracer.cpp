#include <float.h>
#include <iostream>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "random.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "ray_tracer.hpp"

RayTracer::RayTracer(int w, int h, int s) : width(w), height(h), step(s) {
    pixels = new char[w * h * 3];
}

RayTracer::~RayTracer() { delete pixels; }

glm::vec3 RayTracer::getColor(const Ray &ray, Hittable *world, int depth) {
    HitRecord hitRecord;

    if (world->hit(ray, 0.001f, FLT_MAX, hitRecord)) {
        Ray scatterd;
        glm::vec3 attenuation;
        if (depth < 50 && hitRecord.pMaterial->scatter(ray, hitRecord,
                                                       attenuation, scatterd)) {
            return attenuation * getColor(scatterd, world, depth + 1);
        }

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 normalizedDirection = glm::normalize(ray.direction());
    float t = 0.5f * (normalizedDirection[1] + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) +
           t * glm::vec3(0.5f, 0.7f, 1.0f);
}

void RayTracer::render(HittableList *world, Camera *camera) {
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            glm::vec3 color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < step; s++) {
                float u = float(i + getRandom()) / float(width);
                float v = float(j + getRandom()) / float(height);
                Ray ray = camera->getRay(u, v);
                color += getColor(ray, world, 0);
            }

            color /= float(step);
            color = glm::vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

            int ir = int(255.99f * color[0]);
            int ig = int(255.99f * color[1]);
            int ib = int(255.99f * color[2]);

            int offset = (j * width + i) * 3;
            pixels[offset + 0] = ir;
            pixels[offset + 1] = ig;
            pixels[offset + 2] = ib;
        }

        std::cout << "Progress: " << std::fixed << std::setprecision(2)
                  << (100.0f * (height - 1 - j) / (height - 1)) << "%\r"
                  << std::flush;
    }
    std::cout << std::endl;
}
