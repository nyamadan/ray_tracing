#pragma once

#include "ray.hpp"

struct HitRecord {
    float t;

    Vector3 p;
    Vector3 normal;
};

class Hittable {
   public:
    virtual bool hit(const Ray& ray, float tMin, float tMax,
                     HitRecord& hitRecord) const = 0;
};
