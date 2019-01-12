#pragma once

#include "ray.hpp"

class Material;

struct HitRecord {
    float t;
    glm::vec3 p;
    glm::vec3 normal;

    Material* pMaterial;
};

struct Hittable {
   public:
    Hittable() {}
    Hittable(Material* pMat) : pMaterial(pMat) {}
    virtual bool hit(const Ray& ray, float tMin, float tMax,
                     HitRecord& hitRecord) const = 0;
    virtual ~Hittable() {}

    Material* pMaterial;
};
