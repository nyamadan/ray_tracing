#include <random>

#include "random.hpp"

namespace {
std::mt19937 randomEngine;
std::uniform_real_distribution<float> rnd;
}  // namespace

void initializeRandomEngine() {
    std::random_device seed;
    randomEngine = std::mt19937(seed());
    rnd = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

float getRandom() { return rnd(randomEngine); }

glm::vec3 randomInUnitSphere() {
    glm::vec3 p;

    do {
        p = 2.0f * glm::vec3(getRandom(), getRandom(), getRandom()) -
            glm::vec3(1.0f, 1.0f, 1.0f);
    } while (glm::length2(p) < 1.0f);

    return p;
}

glm::vec3 randomInUnitDisk() {
    glm::vec3 p;

    do {
        p = 2.0f * glm::vec3(getRandom(), getRandom(), 0.0f) -
            glm::vec3(1.0f, 1.0f, 0.0f);
    } while (glm::dot(p, p) >= 1.0f);

    return p;
}
