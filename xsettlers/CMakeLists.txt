cmake_minimum_required(VERSION 3.10)

# define project files

set(LOGIC_H 
    logic/map_loader.h
    logic/movable.h
    logic/node.h
    logic/playfield.h)

set(LOGIC_SRC
    logic/map_loader.cpp
    logic/movable.cpp
    logic/playfield.cpp)

set(LOGIC_FILES ${LOGIC_H} ${LOGIC_SRC})

set(LAYER_H
    layer/landscape.h)

set(LAYER_SRC
    layer/landscape.cpp)

set(LAYER_FILES ${LAYER_H} ${LAYER_SRC})

# group files for cleaner project display within some IDEs

source_group("\\" FILES xsettlers.cpp)
source_group("logic\\" FILES ${LOGIC_SRC})
source_group("logic\\headers" FILES ${LOGIC_H})
source_group("layer\\" FILES ${LAYER_SRC})
source_group("layer\\headers" FILES ${LAYER_H})

# append files to current project

set(XSETTLERS_SRC
    xsettlers.cpp
    ${LOGIC_FILES}
    ${LAYER_FILES})

# define target

add_executable(xsettlers ${XSETTLERS_SRC})

# setup target

set_property(TARGET xsettlers PROPERTY CXX_STANDARD 17)

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
