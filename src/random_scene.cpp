#include "random_scene.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "random.hpp"
#include "material.hpp"
#include "sphere.hpp"

RandomScene::RandomScene(float aspect) {
    const glm::vec3 lookFrom(13.0f, 2.0f, 3.0f);
    const glm::vec3 lookAt(0.0f, 0.0f, 0.0f);
    const float distToFocus = 10.0f;
    const float aperture = 0.1f;

    camera = new Camera(lookFrom, lookAt, glm::vec3(0.0f, 1.0f, 0.0f), 20.0f,
                        aspect, aperture, distToFocus);

    const int n = 500;
    Hittable **list = new Hittable *[n + 1];
    list[0] = new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
                         new Lambertian(glm::vec3(0.5f, 0.5f, 0.5f)));

    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float chooseMat = getRandom();
            glm::vec3 center(a + 0.9f * getRandom(), 0.2f,
                             b + 0.9f * getRandom());

            if ((center - glm::vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
                if (chooseMat < 0.8f) {
                    list[i++] = new Sphere(
                        center, 0.2f,
                        new Lambertian(glm::vec3(getRandom() * getRandom(),
                                                 getRandom() * getRandom(),
                                                 getRandom() * getRandom())));
                } else if (chooseMat < 0.95f) {
                    list[i++] = new Sphere(
                        center, 0.2f,
                        new Metal(glm::vec3(0.5f * (1.0f + getRandom()),
                                            0.5f * (1.0f + getRandom()),
                                            0.5f * (1.0f + getRandom())),
                                  0.5f * getRandom()));
                } else {
                    list[i++] = new Sphere(center, 0.2f, new Dielectric(1.5f));
                }
            }
        }
    }

    list[i++] =
        new Sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, new Dielectric(1.5f));
    list[i++] = new Sphere(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f,
                           new Lambertian(glm::vec3(0.4f, 0.2f, 0.1f)));
    list[i++] = new Sphere(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f,
                           new Metal(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f));

    world = new HittableList(list, i);
}

RandomScene::~RandomScene() {
    delete camera;
    for (int i = 0; i < world->listSize; i++) {
        delete world->list[i]->pMaterial;
        delete world->list[i];
    }
    delete[] world->list;
    delete world;
}
