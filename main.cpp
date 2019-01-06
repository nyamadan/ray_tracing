#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#else
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

void update(GLFWwindow *window)
{
    static bool showDemoWindow = false;
    static ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static int displayW, displayH;
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

    for(GLint error = glGetError(); error; error = glGetError()) {
        std::cerr << "error code: " << std::hex << std::showbase << error << std::endl;
    }

    // Rendering
    ImGui::Render();

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &displayW, &displayH);
    glfwGetCursorPos(window, &xpos, &ypos);
    glViewport(0, 0, displayW, displayH);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(window);

    // do something...

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main(void)
{
    GLFWwindow *window;

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
#ifndef __EMSCRIPTEN__
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150 core");
#else
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 300 es");
#endif

    ImGui::StyleColorsDark();

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

#ifndef __EMSCRIPTEN__
    glfwSwapInterval(1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        update(window);
    }

    glfwTerminate();
#else
    emscripten_set_main_loop_arg(reinterpret_cast<em_arg_callback_func>(update), window, 0, 0);
    glfwSwapInterval(1);
#endif

    return 0;
}