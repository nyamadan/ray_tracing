#pragma once

Vector3 randomInUnitSphere();

class Material {
   public:
    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         Vector3& attenuation, Ray& scattered) const = 0;
    virtual ~Material() {}
};

class Lambertian : public Material {
   public:
    Lambertian(const Vector3& a) : albedo(a) {}

    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         Vector3& attenuation, Ray& scattered) const {
        Vector3 target = hitRecord.p + hitRecord.normal + randomInUnitSphere();
        scattered = Ray(hitRecord.p, target - hitRecord.p);
        attenuation = albedo;
        return true;
    }

    Vector3 albedo;
};

Vector3 reflect(const Vector3& v, const Vector3& n) {
    return v - 2 * Vector3::dot(v, n) * n;
}

class Metal : public Material {
   public:
    Metal(const Vector3& a, const float f) : albedo(a), fuzz(f) {}

    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord,
                         Vector3& attenuation, Ray& scattered) const {
        Vector3 reflected =
            reflect(Vector3::normalize(rayIn.direction()), hitRecord.normal);
        scattered = Ray(hitRecord.p, reflected + fuzz * randomInUnitSphere());
        attenuation = albedo;
        return (Vector3::dot(scattered.direction(), hitRecord.normal)) > 0.0f;
    }

    Vector3 albedo;
    float fuzz;
};
