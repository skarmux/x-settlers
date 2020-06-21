cmake_minimum_required(VERSION 3.10)

# project(xsettlers LANGUAGES CXX)

# define project files

set(LOGIC_FILES
    # header    
    logic/map_loader.h
    logic/movable.h
    logic/node.h
    logic/playfield.h
    # source
    logic/map_loader.cpp
    logic/movable.cpp
    logic/playfield.cpp)

set(LAYER_FILES
    game.h
    game.cpp)

set(MAP_FILES
    assets/maps/4Circles.map)

set(TEXTURE_FILES 
    assets/textures/s3_atlas_test.png
    assets/textures/s3_grass.png)

set(OPENGL_SHADER_FILES
    assets/shaders/opengl/texture.glsl)

set(VULKAN_SHADER_FILES
    assets/shaders/vulkan/vert.spv
    assets/shaders/vulkan/frag.spv)

# group files for cleaner project display within some IDEs

source_group("\\" FILES xsettlers.cpp)
source_group("logic\\" FILES ${LOGIC_FILES})
source_group("layer\\" FILES ${LAYER_FILES})
source_group("assets\\maps" FILES ${MAP_FILES})
source_group("assets\\textures" FILES ${TEXTURE_FILES})
source_group("assets\\shaders\\opengl" FILES ${OPENGL_SHADER_FILES})
source_group("assets\\shaders\\vulkan" FILES ${VULKAN_SHADER_FILES})

# append files to current project

set(XSETTLERS_SRC
    xsettlers.cpp
    ${LOGIC_FILES}
    ${LAYER_FILES}
    ${MAP_FILES}
    ${TEXTURE_FILES}
    ${OPENGL_SHADER_FILES}
    ${VULKAN_SHADER_FILES})

# define target

add_executable(xsettlers ${XSETTLERS_SRC})

# setup target

target_link_libraries(xsettlers PUBLIC engine)

target_include_directories(xsettlers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/extern/stb)

# compiler flags based on compiler type

if(MSVC)
    # activate multithreaded processing/compiling
    target_compile_options(xsettlers PUBLIC /MP)
endif()
