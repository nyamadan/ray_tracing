#pragma once

#include "ray.hpp"

class Camera {
   public:
    Camera(float aspectRatio) {
        lowerLeftCorner = glm::vec3(-1.0f * aspectRatio, -1.0f, -1.0f);
        horizontal = glm::vec3(2.0f * aspectRatio, 0.0f, 0.0f);
        vertical = glm::vec3(0.0f, 2.0f, 0.0f);
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
