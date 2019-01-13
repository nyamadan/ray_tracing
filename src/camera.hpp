#pragma once

#include <math.h>

#include "ray.hpp"

class Camera {
   public:
    Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 vup, float vfov,
           float aspect, float aperture, float focusDist) {
        lensRadius = aperture / 2.0f;

        float theta = vfov * M_PI / 180.0f;
        float halfHeight = tanf(theta / 2.0f);
        float halfWidth = aspect * halfHeight;
        origin = lookFrom;
        w = glm::normalize(lookFrom - lookAt);
        u = glm::normalize(glm::cross(vup, w));
        v = glm::cross(w, u);
        lowerLeftCorner = origin - halfWidth * focusDist * u -
                          halfHeight * focusDist * v - focusDist * w;
        horizontal = 2.0f * halfWidth * focusDist * u;
        vertical = 2.0f * halfHeight * focusDist * v;
    }

    Ray getRay(float s, float t) {
        glm::vec3 rd = lensRadius * randomInUnitDisk();
        glm::vec3 offset = u * rd.x + v * rd.y;
        return Ray(origin + offset, lowerLeftCorner + s * horizontal +
                                        t * vertical - origin - offset);
    }

    glm::vec3 origin;
    glm::vec3 lowerLeftCorner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 u, v, w;
    float lensRadius;
};
