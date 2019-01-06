cmake_minimum_required(VERSION 3.0.0)
project(ray_tracing VERSION 0.1.0)

set(APP_SOURCES
    src/main.cpp
    src/shader_utils.cpp
)

set(GL3W_SOURCES
    gl3w/src/gl3w.c
)

set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_widgets.cpp
    imgui/examples/imgui_impl_glfw.cpp
    imgui/examples/imgui_impl_opengl3.cpp
)

#GLM
include_directories(./glm)

# Imgui
include_directories(./imgui)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_EXE_LINKER_FLAGS "-s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1")

    add_executable(ray_tracing ${APP_SOURCES} ${IMGUI_SOURCES})
elseif(MSVC)
    # OpenGL
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS})

    # gl3w
    include_directories(./gl3w/include)

    # GLFW
    add_subdirectory(./glfw)
    include_directories(./glfw/include)

    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_executable(ray_tracing ${APP_SOURCES} ${IMGUI_SOURCES} ${GL3W_SOURCES})
    add_dependencies(ray_tracing glfw)
    target_link_libraries(ray_tracing ${OPENGL_LIBRARIES} glfw)
else(APPLE)
    # OpenGL
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS})

    # gl3w
    include_directories(./gl3w/include)

    # GLFW
    add_subdirectory(./glfw)
    include_directories(./glfw/include)
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/glfw/src)

    add_executable(ray_tracing ${APP_SOURCES} ${IMGUI_SOURCES} ${GL3W_SOURCES})
    target_link_libraries(ray_tracing ${OPENGL_LIBRARIES} glfw)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
