#include <float.h>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stb_image_write.h>

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include "common.hpp"
#include "random.hpp"
#include "shader_utils.hpp"

#include "camera.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

namespace {
const int PositionLocation = 0;
const int PositionSize = 3;

GLFWwindow *window = nullptr;

GLuint vIndex = 0;
GLuint vPosition = 0;
GLuint vertexArraysObject = 0;

GLint uTime = 0;
GLint uMouse = 0;
GLint uResolution = 0;
GLint uImage0 = 0;

GLuint texImage0 = 0;

GLuint program = 0;

char *pixels = nullptr;

const GLfloat positions[] = {-1.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f,
                             -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f};

const GLushort indices[] = {0, 2, 1, 1, 2, 3};

void glfw_error_callback(int error, const char *description) {
    std::cerr << "error " << error << ": " << description << std::endl;
}

glm::vec3 getColor(const Ray &ray, Hittable *world, int depth) {
    HitRecord hitRecord;

    if (world->hit(ray, 0.001f, FLT_MAX, hitRecord)) {
        Ray scatterd;
        glm::vec3 attenuation;
        if (depth < 50 && hitRecord.pMaterial->scatter(ray, hitRecord,
                                                       attenuation, scatterd)) {
            return attenuation * getColor(scatterd, world, depth + 1);
        }

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 normalizedDirection = glm::normalize(ray.direction());
    float t = 0.5f * (normalizedDirection[1] + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) +
           t * glm::vec3(0.5f, 0.7f, 1.0f);
}

HittableList *randomScene() {
    int n = 500;
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

    return new HittableList(list, i);
}

static void update(void *) {
    static bool showDemoWindow = false;
    static bool showDebugWindow = true;
    static int width, height;
    static double xpos, ypos;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (showDebugWindow) {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);

        ImGui::Checkbox("Demo Window",
                        &showDemoWindow);  // Edit bools storing our windows
                                           // open/close state
    }

    if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    ImGui::Render();

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &width, &height);
    glfwGetCursorPos(window, &xpos, &ypos);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    if (uResolution >= 0) {
        glUniform2f(uResolution, static_cast<GLfloat>(width),
                    static_cast<GLfloat>(height));
    }

    if (uMouse >= 0) {
        glUniform2f(uMouse, static_cast<GLfloat>(xpos),
                    static_cast<GLfloat>(ypos));
    }

    if (uImage0 >= 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texImage0);
        glUniform1i(uImage0, 0);
    }

    glBindVertexArray(vertexArraysObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);

    for (GLint error = glGetError(); error; error = glGetError()) {
        std::cerr << "error code: " << std::hex << std::showbase << error
                  << std::endl;
    }

    glfwPollEvents();
}

static void readText(char *&memblock, const char *const path) {
    std::ifstream file(path, std::ios::in | std::ios::ate);

    if (!file.is_open()) {
        return;
    }

    const size_t size = static_cast<size_t>(file.tellg());
    memblock = new char[size + 1];
    file.seekg(0, std::ios::beg);
    file.read(memblock, size);
    memblock[size] = '\0';
    file.close();
}
}  // namespace

int main(void) {
    const int Width = 1200;
    const int Height = 800;
    const int Step = 10;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    initializeRandomEngine();

    // Initialize Textures
    pixels = new char[Width * Height * 3];

    float aspect = float(Width) / float(Height);

    HittableList *world = randomScene();

    glm::vec3 lookFrom(13.0f, 2.0f, 3.0f);
    glm::vec3 lookAt(0.0f, 0.0f, 0.0f);
    float distToFocus = 10.0f;
    float aperture = 0.1f;
    Camera camera(lookFrom, lookAt, glm::vec3(0.0f, 1.0f, 0.0f), 20.0f, aspect,
                  aperture, distToFocus);

    for (int j = Height - 1; j >= 0; j--) {
        for (int i = 0; i < Width; i++) {
            glm::vec3 color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < Step; s++) {
                float u = float(i + getRandom()) / float(Width);
                float v = float(j + getRandom()) / float(Height);
                Ray ray = camera.getRay(u, v);
                glm::vec3 p = ray.pointAtParameter(2.0f);
                color += getColor(ray, world, 0);
            }

            color /= float(Step);
            color = glm::vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

            int ir = int(255.99f * color[0]);
            int ig = int(255.99f * color[1]);
            int ib = int(255.99f * color[2]);

            int offset = (j * Width + i) * 3;
            pixels[offset + 0] = ir;
            pixels[offset + 1] = ig;
            pixels[offset + 2] = ib;
        }

        std::cout << "Progress: " << std::fixed << std::setprecision(2)
                  << (100.0f * (Height - 1 - j) / (Height - 1)) << "%\r"
                  << std::flush;
    }
    std::cout << std::endl;

    for (int i = 0; i < world->listSize; i++) {
        delete world->list[i]->pMaterial;
        delete world->list[i];
    }
    delete world->list;
    delete world;

    // File out.
    const int NumComponents = 3;
    stbi_flip_vertically_on_write(1);
    stbi_write_png("result.png", Width, Height, NumComponents, pixels,
                   NumComponents * Width);

#ifdef __EMSCRIPTEN__
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#endif

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    float scl = 1024.0f / Width;
    window = glfwCreateWindow(int(Width * scl), int(Height * scl),
                              "Ray Tracing", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

#ifndef __EMSCRIPTEN__
    // Initialize OpenGL loader
    if (gl3wInit() != 0) {
        return 1;
    }
#endif

    ImGui::SetCurrentContext(ImGui::CreateContext());
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GlslVersion);

    ImGui::StyleColorsDark();

    glfwMakeContextCurrent(window);

    // Initialize Buffers
    glGenVertexArrays(1, &vertexArraysObject);
    glGenBuffers(1, &vIndex);
    glGenBuffers(1, &vPosition);

    glBindVertexArray(vertexArraysObject);
    glBindBuffer(GL_ARRAY_BUFFER, vPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, positions,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(PositionLocation, PositionSize, GL_FLOAT, GL_FALSE, 0,
                          0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 6, indices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glGenTextures(1, &texImage0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, texImage0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] pixels;

    // Compile shaders.
    char *vsSource = nullptr;
    readText(vsSource, "./assets/basic.vert");

    char *fsSource = nullptr;
    readText(fsSource, "./assets/basic.frag");

    const char *const vsSources[2] = {GlslVersion, vsSource};
    const char *const fsSources[2] = {GlslVersion, fsSource};

    const GLuint handleVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(handleVS, 2, vsSources, NULL);
    glCompileShader(handleVS);
    assert(checkCompiled(handleVS));

    const GLuint handleFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(handleFS, 2, fsSources, NULL);
    glCompileShader(handleFS);
    assert(checkCompiled(handleFS));

    program = glCreateProgram();
    glAttachShader(program, handleVS);
    glAttachShader(program, handleFS);
    glLinkProgram(program);
    assert(checkLinked(program));

    uImage0 = glGetUniformLocation(program, "image0");
    uTime = glGetUniformLocation(program, "time");
    uMouse = glGetUniformLocation(program, "mouse");
    uResolution = glGetUniformLocation(program, "resolution");

    glDeleteShader(handleVS);
    glDeleteShader(handleFS);

    delete[] vsSource;
    delete[] fsSource;

#ifndef __EMSCRIPTEN__
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        update(nullptr);
    }

    glfwTerminate();
#else
    emscripten_set_main_loop_arg(update, nullptr, 0, 0);
    glfwSwapInterval(1);
#endif

    return 0;
}
