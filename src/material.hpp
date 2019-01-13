#pragma once

glm::vec3 randomInUnitSphere();

class Material {
   public:
    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const = 0;
    virtual ~Material() {}
};

class Lambertian : public Material {
   public:
    Lambertian(const glm::vec3& a) : albedo(a) {}

    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const {
        auto target = hitRecord.p + hitRecord.normal + randomInUnitSphere();
        scattered = Ray(hitRecord.p, target - hitRecord.p);
        attenuation = albedo;
        return true;
    }

    glm::vec3 albedo;
};

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) {
    return v - 2 * glm::dot(v, n) * n;
}

class Metal : public Material {
   public:
    Metal(const glm::vec3& a, const float f) : albedo(a), fuzz(f) {}

    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const {
        auto reflected =
            reflect(glm::normalize(rayIn.direction()), hitRecord.normal);
        scattered = Ray(hitRecord.p, reflected + fuzz * randomInUnitSphere());
        attenuation = albedo;
        return (glm::dot(scattered.direction(), hitRecord.normal)) > 0.0f;
    }

    glm::vec3 albedo;
    float fuzz;
};

bool refract(const glm::vec3& v, const glm::vec3& n, float niOverNt,
             glm::vec3& refracted) {
    auto uv = glm::normalize(v);
    auto dt = glm::dot(uv, n);
    auto discriminant = 1.0f - niOverNt * niOverNt * (1.0f - dt * dt);

    if (discriminant > 0.0f) {
        refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }

    return false;
}

class Dielectric : public Material {
   public:
    Dielectric(float ri) : refIdx(ri) {}

    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const {
        glm::vec3 outwardNormal;
        auto reflected = reflect(rayIn.direction(), hitRecord.normal);
        float niOverNt;
        attenuation = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 refracted;
        if (glm::dot(rayIn.direction(), hitRecord.normal) > 0) {
            outwardNormal = -hitRecord.normal;
            niOverNt = refIdx;
        } else {
            outwardNormal = hitRecord.normal;
            niOverNt = 1.0f / refIdx;
        }

        if (refract(rayIn.direction(), outwardNormal, niOverNt, refracted) >
            0.0f) {
            scattered = Ray(hitRecord.p, refracted);
        } else {
            scattered = Ray(hitRecord.p, reflected);
        }

        return true;
    }

    float refIdx;
};

float schlick(float cosine, float refIdx) {
    auto r0 = (1.0f - refIdx) / (1.0f + refIdx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1 - cosine), 5);
}
