cmake_minimum_required(VERSION 3.10)

project(xsettlers LANGUAGES CXX)

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

set(MAP_FILES
    assets/maps/4Circles.map)

set(TEXTURE_FILES 
    assets/textures/s3_atlas_test.png
    assets/textures/s3_grass.png)

# group files for cleaner project display within some IDEs

source_group("\\" FILES xsettlers.cpp)
source_group("logic\\" FILES ${LOGIC_FILES})
source_group("assets\\maps" FILES ${MAP_FILES})
source_group("assets\\textures" FILES ${TEXTURE_FILES})

# append files to current project

set(XSETTLERS_SRC
    xsettlers.cpp
    ${LOGIC_FILES}
    ${MAP_FILES}
    ${TEXTURE_FILES})

# define target

add_executable(${PROJECT_NAME} ${XSETTLERS_SRC})

# setup target

target_link_libraries(xsettlers PUBLIC engine)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/third_party/glm
    ${CMAKE_SOURCE_DIR}/third_party/stb)

# compiler flags based on compiler type
if(MSVC)
    # activate multithreaded processing/compiling
    target_compile_options(${PROJECT_NAME} PUBLIC /MP)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -fexceptions)
endif()
