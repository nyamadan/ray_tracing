#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "random.hpp"
#include "hittable.hpp"
#include "material.hpp"

namespace {
glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) {
    return v - 2 * glm::dot(v, n) * n;
}

bool refract(const glm::vec3& v, const glm::vec3& n, float niOverNt,
             glm::vec3& refracted) {
    glm::vec3 uv = glm::normalize(v);
    float dt = glm::dot(uv, n);
    float discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);

    if (discriminant > 0.0f) {
        refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }

    return false;
}

float schlick(float cosine, float refIdx) {
    float r0 = (1.0f - refIdx) / (1.0f + refIdx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1 - cosine), 5);
}
}  // namespace

Lambertian::Lambertian(const glm::vec3& a) : albedo(a) {}

bool Lambertian::scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const {
    glm::vec3 target = hitRecord.p + hitRecord.normal + randomInUnitSphere();
    scattered = Ray(hitRecord.p, target - hitRecord.p);
    attenuation = albedo;
    return true;
}

Metal::Metal(const glm::vec3& a, const float f) : albedo(a), fuzz(f) {}

bool Metal::scatter(const Ray& rayIn, const HitRecord& hitRecord,
                    glm::vec3& attenuation, Ray& scattered) const {
    glm::vec3 reflected =
        reflect(glm::normalize(rayIn.direction()), hitRecord.normal);
    scattered = Ray(hitRecord.p, reflected + fuzz * randomInUnitSphere());
    attenuation = albedo;
    return (glm::dot(scattered.direction(), hitRecord.normal)) > 0.0f;
}

Dielectric::Dielectric(float ri) : refIdx(ri) {}

bool Dielectric::scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const {
    glm::vec3 outwardNormal;
    glm::vec3 reflected = reflect(rayIn.direction(), hitRecord.normal);
    float niOverNt;
    attenuation = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 refracted;
    float reflectProb;
    float cosine;
    if (glm::dot(rayIn.direction(), hitRecord.normal) > 0.0f) {
        outwardNormal = -hitRecord.normal;
        niOverNt = refIdx;
        cosine = glm::dot(rayIn.direction(), hitRecord.normal) /
                 rayIn.direction().length();
        cosine = sqrt(1.0f - refIdx * refIdx * (1.0f - cosine * cosine));
    } else {
        outwardNormal = hitRecord.normal;
        niOverNt = 1.0f / refIdx;
        cosine = -glm::dot(rayIn.direction(), hitRecord.normal) /
                 rayIn.direction().length();
    }

    if (refract(rayIn.direction(), outwardNormal, niOverNt, refracted)) {
        reflectProb = schlick(cosine, refIdx);
    } else {
        reflectProb = 1.0f;
    }

    if (getRandom() < reflectProb) {
        scattered = Ray(hitRecord.p, reflected);
    } else {
        scattered = Ray(hitRecord.p, refracted);
    }

    return true;
};
