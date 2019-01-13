#pragma once

#include <math.h>

#include "ray.hpp"

class Camera {
   public:
    Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 vup, float vfov, float aspect) {
        glm::vec3 u, v, w;
        float theta = vfov * M_PI / 180.0f;
        float halfHeight = tanf(theta / 2.0f);
        float halfWidth = aspect * halfHeight;

        origin = lookFrom;
        w = glm::normalize(lookFrom - lookAt);
        u = glm::normalize(glm::cross(vup, w));
        v = glm::cross(w, u);

        lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
        horizontal = 2.0f * halfWidth * u;
        vertical = 2.0f * halfHeight * v;
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
