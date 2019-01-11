#pragma once

#include "hittable.hpp"

class Sphere : public Hittable {
   public:
    Sphere() {}
    Sphere(Vector3 cen, float r) : center(cen), radius(r) {}

    bool hit(const Ray& ray, float tMin, float tMax,
             HitRecord& hitRecord) const;

    Vector3 center;
    float radius;
};

bool Sphere::hit(const Ray& ray, float tMin, float tMax,
                 HitRecord& hitRecord) const {
    Vector3 oc = ray.origin() - center;
    float a = dot(ray.direction(), ray.direction());
    float b = dot(oc, ray.direction());
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;

        if (temp < tMax && temp > tMin) {
            hitRecord.t = temp;
            hitRecord.p = ray.pointAtParameter(hitRecord.t);
            hitRecord.normal = (hitRecord.p - center) / radius;
            return true;
        }

        temp = (-b + sqrt(discriminant)) / a;
        if (temp < tMax && temp > tMin) {
            hitRecord.t = temp;
            hitRecord.p = ray.pointAtParameter(hitRecord.t);
            hitRecord.normal = (hitRecord.p - center) / radius;
            return true;
        }
    }

    return false;
}
