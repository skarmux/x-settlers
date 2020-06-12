cmake_minimum_required(VERSION 3.10)

project(engine LANGUAGES CXX)

# define project variables

set(ENGINE_RENDER_API "OpenGL" CACHE STRING "Graphics API for renderer")
set(CACHE ENGINE_RENDER_API PROPERTY STRINGS "OpenGL;Vulkan;DirectX;Metal")

# define project files

set(CORE_FILES
    # header
    core/layer.h
    core/layer_stack.h
    core/log.h
    # source
    core/layer.cpp
    core/layer_stack.cpp
    core/log.cpp)

set(RENDERING_FILES
    # header
    rendering/buffer.h
    rendering/graphics_context.h
    rendering/render_command.h
    rendering/renderer_2d.h
    rendering/renderer_api.h
    rendering/shader.h
    rendering/texture.h
    rendering/vertex.h
    # source
    rendering/buffer.cpp
    rendering/graphics_context.cpp
    rendering/render_command.cpp
    rendering/renderer_2d.cpp
    rendering/renderer_api.cpp
    rendering/shader.cpp)

set(PLATFORM_FILES
    # header
    platform/filesystem.h
    platform/window.h
    # source
    platform/window.cpp)

set(ANDROID_FILES
    # header
    platform/android/android_window.h
    # source
    platform/android/android_window.cpp)

set(GLFW_FILES
    # ...
    )

set(SDL_FILES
    # header
    platform/sdl_window.h
    # source
    platform/sdl_window.cpp)

set(OPENGL_FILES
    # header
    platform/opengl/opengl_buffer.h
    platform/opengl/opengl_context.h
    platform/opengl/opengl_renderer_api.h
    platform/opengl/opengl_shader.h
    # source
    platform/opengl/opengl_buffer.cpp
    platform/opengl/opengl_context.cpp
    platform/opengl/opengl_renderer_api.cpp
    platform/opengl/opengl_shader.cpp)

set(OPENGL_SHADER_FILES
    shaders/opengl/texture.glsl)

set(VULKAN_SHADER_FILES
    shaders/vulkan/vert.spv
    shaders/vulkan/frag.spv)

set(GLES_FILES
    # header
    platform/gles/gles_context.h
    platform/gles/gles_renderer_api.h
    # source
    platform/gles/gles_context.cpp
    platform/gles/gles_renderer_api.cpp)

set(VULKAN_FILES
    # header
    platform/vulkan/vulkan_buffer.h
    platform/vulkan/vulkan_context.h
    platform/vulkan/vulkan_renderer_api.h)

# group files for cleaner project display within some IDEs

source_group("\\" FILES engine.h)
source_group("core\\" FILES ${CORE_FILES})
source_group("shaders\\opengl" FILES ${OPENGL_SHADER_FILES})
source_group("shaders\\vulkan" FILES ${VULKAN_SHADER_FILES})
source_group("rendering\\" FILES ${RENDERING_FILES})
source_group("platform\\" FILES ${PLATFORM_FILES} ${SDL_FILES} ${GLFW_FILES})
source_group("platform\\android" FILES ${ANDROID_FILES})
source_group("platform\\opengl" FILES ${OPENGL_FILES})
source_group("platform\\gles" FILES ${GLES_FILES})
source_group("platform\\vulkan" FILES ${VULKAN_FILES})

# append files to current project

set(PROJECT_FILES
    engine.h
    ${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c
    ${CORE_FILES}
    ${PLATFORM_FILES}
    ${RENDERING_FILES})

if(ANDROID)
    list(APPEND PROJECT_FILES ${ANDROID_FILES})
else()
    list(APPEND PROJECT_FILES ${SDL_FILES})
endif()

if(ENGINE_RENDER_API STREQUAL "OpenGL")
    list(APPEND PROJECT_FILES ${OPENGL_FILES})
    list(APPEND PROJECT_FILES ${OPENGL_SHADER_FILES})
elseif(ENGINE_RENDER_API STREQUAL "Vulkan")
    list(APPEND PROJECT_FILES ${VULKAN_FILES})
    list(APPEND PROJECT_FILES ${VULKAN_SHADER_FILES})
endif()

# define target as library

add_library(${PROJECT_NAME} ${PROJECT_FILES})

# setup target

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/third_party/glm
    ${CMAKE_SOURCE_DIR}/third_party/spdlog/include
    ${CMAKE_SOURCE_DIR}/third_party/stb)

target_link_libraries(${PROJECT_NAME} spdlog stb)

if(ENGINE_RENDER_API STREQUAL "OpenGL")

    message(STATUS "Adding OpenGL API to project.")

    if(ANDROID)
        # OpenGL ES for mobile
        #find_package(OpenGLES REQUIRED)
        target_link_libraries(${PROJECT_NAME} EGL GLESv1_CM)
        #list(APPEND PROJECT_FILES ${OPENGL_ES_FILES})
    else()
        # OpenGL for desktop
        find_package(OpenGL REQUIRED)
        target_include_directories(${PROJECT_NAME} PUBLIC
            ${CMAKE_SOURCE_DIR}/third_party/glad/include)
    endif()

elseif(ENGINE_RENDER_API STREQUAL "Vulkan")

    message(STATUS "Adding Vulkan API to project.")
    find_package(Vulkan REQUIRED)    
    # target_link_libraries(${PROJECT_NAME} vulkan)

else()
    message(ERROR "Selected graphics API '${ENGINE_RENDER_API}' currently not supported.")

endif()

if(ANDROID)

    # add native-app dependencies
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)
    target_link_libraries(${PROJECT_NAME}
        log android	native_app_glue)

else()

    # add SDL2 requirement
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} SDL2main SDL2)

    elseif(UNIX)
        find_package(SDL2 REQUIRED)

        target_include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

    else()
        message(FATAL_ERROR "Unsupported platform detected.")

    endif()

endif()
