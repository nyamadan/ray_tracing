#pragma once

#include "ray.hpp"

class Camera {
   public:
    Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 vup, float vfov,
           float aspect, float aperture, float focusDist);

    Ray getRay(float s, float t);

    glm::vec3 origin;
    glm::vec3 lowerLeftCorner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 u, v, w;
    float lensRadius;
};
