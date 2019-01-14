#pragma once

#include "hittable.hpp"

class HittableList : public Hittable {
   public:
    HittableList();
    HittableList(Hittable **l, int n);

    virtual bool hit(const Ray &r, float tMin, float tMax,
                     HitRecord &rec) const;

    Hittable **list;
    int listSize;
};
