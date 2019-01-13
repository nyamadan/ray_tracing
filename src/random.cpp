#include <random>

#include "random.hpp"

namespace {
std::mt19937 randomEngine;
std::uniform_real_distribution<float> random;
}  // namespace

void initializeRandomEngine() {
    std::random_device seed;
    randomEngine = std::mt19937(seed());
    random = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

float getRandom() { return random(randomEngine); }

glm::vec3 randomInUnitSphere() {
    glm::vec3 p;

    do {
        p = glm::vec3(2.0f * getRandom() - 1.0f, 2.0f * getRandom() - 1.0f,
                      2.0f * getRandom() - 1.0f);
    } while (glm::length2(p) < 1.0f);

    return p;
}
