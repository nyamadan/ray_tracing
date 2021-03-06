#pragma once

class RayTracer {
   public:
    RayTracer(int w, int h, int s);
    ~RayTracer();
    glm::vec3 getColor(const Ray &ray, Hittable *world, int depth);
    void render(HittableList *world, Camera *camera);
    void renderPixel(HittableList *world, Camera *camera, int i, int j);

    int width;
    int height;
    int step;
    char *pixels;
};
