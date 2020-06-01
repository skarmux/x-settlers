cmake_minimum_required(VERSION 2.8.5)
project(GLAD LANGUAGES C)

add_library(glad src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
