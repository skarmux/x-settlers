cmake_minimum_required(VERSION 3.10)

# set the solution name
project(X-Settlers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT xsettlers)

# add thrid party libraries
add_subdirectory(extern)

if(WIN32)
    # use precompiled SDL2 binaries
    include_directories(${CMAKE_SOURCE_DIR}/extern/SDL2/include)
    link_directories(${CMAKE_SOURCE_DIR}/extern/SDL2/lib/x64)
endif()

# add engine
add_subdirectory(engine)

# add game
add_subdirectory(xsettlers)

if(ANDROID)
    # add xsettlers app
    #add_subdirectory(app)
endif()
