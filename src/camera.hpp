#pragma once

#include <math.h>

#include "ray.hpp"

class Camera {
   public:
    Camera(float fov, float aspect) {
        float theta = fov * M_PI / 180.0f;
        float halfHeight = tanf(theta / 2.0f);
        float halfWidth = aspect * halfHeight;

        lowerLeftCorner = glm::vec3(-halfWidth, -halfHeight, -1.0f);
        horizontal = glm::vec3(2.0f * halfWidth, 0.0f, 0.0f);
        vertical = glm::vec3(0.0f, 2.0f * halfHeight, 0.0f);
        origin = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    Ray getRay(float u, float v) {
        return Ray(origin,
                   lowerLeftCorner + u * horizontal + v * vertical - origin);
    }

    glm::vec3 origin;
    glm::vec3 lowerLeftCorner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
};
