#pragma once

#include "hittable.hpp"

class Sphere : public Hittable {
   public:
    Sphere() {}
    Sphere(glm::vec3 cen, float r, Material* pMat)
        : Hittable(pMat), center(cen), radius(r) {}

    bool hit(const Ray& ray, float tMin, float tMax,
             HitRecord& hitRecord) const;

    glm::vec3 center;
    float radius;
};
