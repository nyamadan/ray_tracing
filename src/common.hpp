#pragma once

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

#include "./vector3.hpp"
