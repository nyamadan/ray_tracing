#pragma once

#include "ray.hpp"

class Camera {
   public:
    Camera(float aspectRatio) {
        lowerLeftCorner = Vector3(-1.0f * aspectRatio, -1.0f, -1.0f);
        horizontal = Vector3(2.0f * aspectRatio, 0.0f, 0.0f);
        vertical = Vector3(0.0f, 2.0f, 0.0f);
        origin = Vector3(0.0f, 0.0f, 0.0f);
    }

    Ray getRay(float u, float v) {
        return Ray(origin,
                   lowerLeftCorner + u * horizontal + v * vertical - origin);
    }

    Vector3 origin;
    Vector3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;
};
