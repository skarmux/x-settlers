cmake_minimum_required(VERSION 3.10)

# glm
add_library(glm INTERFACE)
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)
target_sources(glm INTERFACE ${GLM_DIR}/glm/glm.hpp)
target_include_directories(glm INTERFACE ${GLM_DIR})

target_compile_definitions(glm INTERFACE
    GLM_FORCE_SWIZZLE
    GLM_FORCE_RADIANS
    GLM_FORCE_CTOR_INIT
    GLM_ENABLE_EXPERIMENTAL
)
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_definitions(glm INTERFACE GLM_FORCE_CXX14)
endif()

#glad
add_subdirectory(glad)
set_property(TARGET glad PROPERTY FOLDER "extern")

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb")

if(ANDROID)
    # native_app_glue
    set(NATIVE_APP_GLUE_DIR "${CMAKE_ANDROID_NDK}/sources/android/native_app_glue")

    file(GLOB NATIVE_APP_GLUE_FILES "${NATIVE_APP_GLUE_DIR}/*.c" "${NATIVE_APP_GLUE_DIR}/*.h")
    
    add_library(native_app_glue STATIC ${NATIVE_APP_GLUE_FILES})
        
    target_include_directories(native_app_glue PUBLIC ${NATIVE_APP_GLUE_DIR})
    
    set_property(TARGET native_app_glue PROPERTY FOLDER "extern")
endif()

# spdlog
add_subdirectory(spdlog)
set_property(TARGET spdlog PROPERTY FOLDER "extern")
