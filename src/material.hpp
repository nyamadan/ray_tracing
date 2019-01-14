#pragma once

struct HitRecord;

class Material {
   public:
    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const = 0;
    virtual ~Material() {}
};

class Lambertian : public Material {
   public:
    Lambertian(const glm::vec3& a);

    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const;

    glm::vec3 albedo;
};

class Metal : public Material {
   public:
    Metal(const glm::vec3& a, const float f);

    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const;

    glm::vec3 albedo;
    float fuzz;
};

class Dielectric : public Material {
   public:
    Dielectric(float ri);

    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         glm::vec3& attenuation, Ray& scattered) const;

    float refIdx;
};
