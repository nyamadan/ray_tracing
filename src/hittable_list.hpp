#pragma once

#include "hittable.hpp"

class HittableList : public Hittable {
   public:
    HittableList() {}
    HittableList(Hittable **l, int n) {
        list = l;
        listSize = n;
    }

    virtual bool hit(const Ray &r, float tMin, float tMax,
                     HitRecord &rec) const {
        HitRecord tempRec;
        bool hitAnything = false;
        float closestSoFar = tMax;

        for (int i = 0; i < listSize; i++) {
            if (list[i]->hit(r, tMin, closestSoFar, tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }

    Hittable **list;
    int listSize;
};
