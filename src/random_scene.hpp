#pragma once

#include "camera.hpp"
#include "hittable_list.hpp"

class RandomScene {
   public:
    RandomScene(float aspect);

    ~RandomScene();

    Camera *camera;
    HittableList *world;
};
