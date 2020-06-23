cmake_minimum_required(VERSION 3.10)

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
    # header
    layer/game.h
    layer/landscape.h
    layer/interface.h
    # source
    layer/game.cpp
    layer/landscape.cpp
    layer/interface.cpp)

# group files for cleaner project display within some IDEs

source_group("\\" FILES xsettlers.cpp)
source_group("logic\\" FILES ${LOGIC_FILES})
source_group("layer\\" FILES ${LAYER_FILES})

# append files to current project

set(XSETTLERS_SRC
    xsettlers.cpp
    ${LOGIC_FILES}
    ${LAYER_FILES})

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
