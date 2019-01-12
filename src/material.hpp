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
        glm::vec3 target =
            hitRecord.p + hitRecord.normal + randomInUnitSphere();
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
        glm::vec3 reflected =
            reflect(glm::normalize(rayIn.direction()), hitRecord.normal);
        scattered = Ray(hitRecord.p, reflected + fuzz * randomInUnitSphere());
        attenuation = albedo;
        return (glm::dot(scattered.direction(), hitRecord.normal)) > 0.0f;
    }

    glm::vec3 albedo;
    float fuzz;
};
