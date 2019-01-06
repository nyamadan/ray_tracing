#include "common.hpp"
#include <iostream>

#include "shader_utils.hpp"

static const char* const VsSource =
    "layout (location = 0) in vec3 aPosition;\n"
    "\n"
    "void main(void){\n"
    "  gl_Position = vec4(aPosition, 1.0);\n"
    "}";

static const char* const FsSource =
    "precision mediump float;\n"
    "\n"
    "uniform vec2 resolution;\n"
    "\n"
    "out vec4 fragColor;\n"
    "\n"
    "void main(void){\n"
    "  vec2 p = gl_FragCoord.xy / resolution;\n"
    "  vec3 rgb = vec3(p, 0.0);\n"
    "  fragColor = vec4(rgb, 1.0);\n"
    "}";

static const int PositionLocation = 0;
static const int PositionSize = 3;

static GLFWwindow *window = nullptr;

static GLuint vIndex = 0;
static GLuint vPosition = 0;
static GLuint vertexArraysObject = 0;
static GLint uResolution = 0;
static GLuint program = 0;

static const GLfloat positions[] = {
    -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f
};

static const GLushort indices[] = {
    0, 2, 1,
    1, 2, 3
};

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "Error " << error << ": " << description << std::endl;
}

static void update(void *)
{
    static bool showDemoWindow = false;
    static ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static int width, height;
    static double xpos, ypos;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::ColorEdit3("clear color", (float *)&clearColor); // Edit 3 floats representing a color

    bool needsReload = false;

    ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our windows open/close state

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    // Rendering
    ImGui::Render();

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &width, &height);
    glfwGetCursorPos(window, &xpos, &ypos);
    glViewport(0, 0, width, height);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(window);

    glUseProgram(program);
    glUniform2f(uResolution, static_cast<GLfloat>(width), static_cast<GLfloat>(height));
    glBindVertexArray(vertexArraysObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glfwSwapBuffers(window);

    for(GLint error = glGetError(); error; error = glGetError()) {
        std::cerr << "error code: " << std::hex << std::showbase << error << std::endl;
    }

    glfwPollEvents();
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

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

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(1024, 768, "Ray Tracing", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

#ifndef __EMSCRIPTEN__
    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader
                      // is likely to requires some form of initialization.
#endif
    if (err)
    {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, positions, GL_STATIC_DRAW);
    glVertexAttribPointer(PositionLocation, PositionSize, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 6, indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Compile shaders.
    const char * const vsSources[2] = {GlslVersion, VsSource};
    const char * const fsSources[2] = {GlslVersion, FsSource};

    const GLuint handleVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(handleVS, 2, vsSources, NULL);
    glCompileShader(handleVS);
    checkCompiled(handleVS);

    const GLuint handleFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(handleFS, 2, fsSources, NULL);
    glCompileShader(handleFS);
    assert(checkCompiled(handleFS));

    program = glCreateProgram();
    glAttachShader(program, handleVS);
    glAttachShader(program, handleFS);
    glLinkProgram(program);
    checkLinked(program);

    uResolution = glGetUniformLocation(program, "resolution");

    glDeleteShader(handleVS);
    glDeleteShader(handleFS);

#ifndef __EMSCRIPTEN__
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        update(nullptr);
    }

    glfwTerminate();
#else
    emscripten_set_main_loop_arg(update, nullptr, 0, 0);
    glfwSwapInterval(1);
#endif

    return 0;
}
