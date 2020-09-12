cmake_minimum_required(VERSION 3.16)

# project(engine LANGUAGES CXX)

# define project variables

set(ENGINE_RENDER_API "OpenGL" CACHE STRING "Graphics API for renderer")
set(CACHE ENGINE_RENDER_API PROPERTY STRINGS "OpenGL;Vulkan;DirectX;Metal")

# define project files

set(CORE_H
    core/application.h
    core/layer.h
    core/layer_stack.h
    core/logger.h
    core/entry_point.h
    core/time_delta.h
    core/window.h)

set(CORE_SRC
    core/application.cpp
    core/layer.cpp
    core/layer_stack.cpp
    core/logger.cpp
    core/window.cpp)

set(CORE_FILES ${CORE_H} ${CORE_SRC})

set(EVENT_FILES
    events/application_event.h
    events/event.h
    events/mouse_event.h)

set(RENDERING_H
    rendering/buffer.h
    rendering/orthographic_camera.h
    rendering/orthographic_camera_controller.h
    rendering/render_command.h
    rendering/renderer_2d.h
    rendering/renderer_api.h
    rendering/shader.h
    rendering/texture.h
    rendering/texture_atlas.h
    rendering/vertex_array.h)

set(RENDERING_SRC
    rendering/buffer.cpp
    rendering/orthographic_camera.cpp
    rendering/orthographic_camera_controller.cpp
    rendering/render_command.cpp
    rendering/renderer_2d.cpp
    rendering/renderer_api.cpp
    rendering/shader.cpp
    rendering/texture.cpp
    rendering/texture_atlas.cpp
    rendering/vertex_array.cpp)

set(RENDERING_FILES ${RENDERING_H} ${RENDERING_SRC})

set(PLATFORM_FILES
    platform/filesystem.h
    platform/filesystem.cpp)

set(ANDROID_FILES
    platform/android/android_window.h
    platform/android/android_window.cpp)

set(WINDOWS_FILES
    platform/windows/windows_window.h
    platform/windows/windows_window.cpp)

set(SDL_FILES
    platform/sdl_window.h
    platform/sdl_window.cpp)

set(GLFW_FILES
    platform/glfw_window.h
    platform/glfw_window.cpp)

set(OPENGL_H
    platform/opengl/opengl_buffer.h
    platform/opengl/opengl_renderer_api.h
    platform/opengl/opengl_shader.h
    platform/opengl/opengl_texture.h
    platform/opengl/opengl_vertex_array.h)

set(OPENGL_SRC
    platform/opengl/opengl_buffer.cpp
    platform/opengl/opengl_renderer_api.cpp
    platform/opengl/opengl_shader.cpp
    platform/opengl/opengl_texture.cpp
    platform/opengl/opengl_vertex_array.cpp)

set(OPENGL_FILES ${OPENGL_H} ${OPENGL_SRC})

set(GLES_FILES
    platform/gles/gles_renderer_api.h
    platform/gles/gles_renderer_api.cpp)

set(VULKAN_FILES
    platform/vulkan/vulkan_buffer.h
    platform/vulkan/vulkan_renderer_api.h)

# group files for cleaner project display within some IDEs

source_group("\\" FILES engine.h)
source_group("core\\" FILES ${CORE_SRC})
source_group("core\\headers" FILES ${CORE_H})
source_group("events\\" FILES ${EVENT_FILES})
source_group("rendering\\" FILES ${RENDERING_SRC})
source_group("rendering\\headers" FILES ${RENDERING_H})
source_group("platform\\" FILES ${PLATFORM_FILES} ${SDL_FILES} ${GLFW_FILES})
source_group("platform\\windows" FILES ${WINDOWS_FILES})
source_group("platform\\android" FILES ${ANDROID_FILES})
source_group("platform\\opengl" FILES ${OPENGL_SRC})
source_group("platform\\opengl\\headers" FILES ${OPENGL_H})
source_group("platform\\gles" FILES ${GLES_FILES})
source_group("platform\\vulkan" FILES ${VULKAN_FILES})

# append files to current project

set(PROJECT_FILES
    engine.h
    ${CORE_FILES}
    ${EVENT_FILES}
    ${PLATFORM_FILES}
    ${RENDERING_FILES})

if(ANDROID)
    list(APPEND PROJECT_FILES ${ANDROID_FILES})
endif()

if(WIN32)
    list(APPEND PROJECT_FILES ${WINDOWS_FILES})
endif()

list(APPEND PROJECT_FILES ${SDL_FILES})
list(APPEND PROJECT_FILES ${GLFW_FILES})

if(ENGINE_RENDER_API STREQUAL "OpenGL")
    list(APPEND PROJECT_FILES ${OPENGL_FILES})
elseif(ENGINE_RENDER_API STREQUAL "Vulkan")
    list(APPEND PROJECT_FILES ${VULKAN_FILES})
endif()

# define target as library

add_library(engine STATIC ${PROJECT_FILES})

# setup target

set_property(TARGET engine PROPERTY CXX_STANDARD 17)

target_include_directories(engine PUBLIC
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(engine spdlog glad stb glfw)

# https://gitlab.kitware.com/cmake/cmake/merge_requests/3553
target_precompile_headers(engine
    PRIVATE
        [["core/logger.h"]]
        [["platform/filesystem.h"]]
        <string>
        <fstream>
        <iostream>
        <memory>
        <array>
        <vector>
        <algorithm>
        <functional>
        <filesystem>
        <unordered_map>
)

if(ENGINE_RENDER_API STREQUAL "OpenGL")

    message(STATUS "Adding OpenGL API to project.")

    if(ANDROID)
        # OpenGL ES for mobile
        #find_package(OpenGLES REQUIRED)
        target_link_libraries(engine EGL GLESv1_CM)
        #list(APPEND PROJECT_FILES ${OPENGL_ES_FILES})
    else()
        # OpenGL for desktop
        find_package(OpenGL REQUIRED)
        #target_include_directories(${PROJECT_NAME} PUBLIC
        #    ${CMAKE_SOURCE_DIR}/third_party/glad/include)
    endif()

elseif(ENGINE_RENDER_API STREQUAL "Vulkan")

    message(STATUS "Adding Vulkan API to project.")
    find_package(Vulkan REQUIRED)
    target_link_libraries(engine vulkan)

else()
    message(ERROR "Selected graphics API '${ENGINE_RENDER_API}' currently not supported.")

endif()

if(ANDROID)

    # add native-app dependencies
    target_include_directories(engine PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)
    target_link_libraries(engine
        log android	native_app_glue)

else()

    # add SDL2 requirement
    if(WIN32)
        target_link_libraries(engine SDL2main SDL2)

    elseif(UNIX)
        find_package(SDL2 REQUIRED)

        target_include_directories(engine ${SDL2_INCLUDE_DIRS})
        target_link_libraries(engine ${SDL2_LIBRARIES})

    else()
        message(FATAL_ERROR "Unsupported platform detected.")

    endif()

endif()

# compiler flags based on compiler type

if(MSVC)
    # activate multithreaded processing/compiling
    target_compile_options(engine PUBLIC /MP)
endif()
