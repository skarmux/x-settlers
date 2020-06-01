cmake_minimum_required(VERSION 3.10)

# set the project name
project(X-Settlers VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add source files
file(GLOB_RECURSE SOURCE_FILES
	"src/*.c"
	"src/*.cpp"
	"src/*.h"
	"src/*.hpp"
)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/thirdparty/glm"
	"${CMAKE_SOURCE_DIR}/thirdparty/spdlog/include"
	"${CMAKE_SOURCE_DIR}/thirdparty/stb"
)
if(WIN32)
	target_link_libraries(${PROJECT_NAME} mingw32) # Must be before SDL
endif()

# Platform
set(PLATFORM_DEFAULT "Desktop")
set(PLATFORM_TYPES "Desktop" "Android")
set(PLATFORM ${PLATFORM_DEFAULT} CACHE STRING "Project platform, default: ${PLATFORM_DEFAULT}")
set_property(CACHE PLATFORM PROPERTY STRINGS ${PLATFORM_TYPES})
# TODO: make actual use of it

# SDL - assume that user has it installed in OS
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(
	${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static
)

# Renderer
set(RENDERER_DEFAULT "OpenGL")
set(RENDERER_TYPES "OpenGL" "Vulkan")
set(RENDERER ${RENDERER_DEFAULT} CACHE STRING "Project renderer, default: ${RENDERER_DEFAULT}")
set_property(CACHE RENDERER PROPERTY STRINGS ${RENDERER_TYPES})

message(STATUS "Using '${RENDERER}' renderer")
if(RENDERER MATCHES Vulkan)
	find_package(Vulkan REQUIRED)
	# TODO: target_link_libraries(something)
	target_compile_definitions(${PROJECT_NAME} PRIVATE RENDERER=Vulkan)
elseif(RENDERER MATCHES OpenGL)
	find_package(OpenGL REQUIRED)
	add_subdirectory("thirdparty/glad")
	target_link_libraries(${PROJECT_NAME} glad)
	target_compile_definitions(${PROJECT_NAME} PRIVATE RENDERER=OpenGL)
else()
	message(FATAL_ERROR "Couldn't deduce renderer! Please install OpenGL or Vulkan")
endif()

if(UNIX)
	set(LIBS GL X11 Xrandr Xi dl pthread)
endif()

# Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
	# Macro to preserve source files hierarchy in the IDE
	macro(GroupSources curdir)
		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				string(REPLACE "src" "Sources" groupname ${groupname})
				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()

	# Run macro
	GroupSources(src)
endif()

# copy dlls
#if(WIN32)
#	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#		COMMAND ${CMAKE_COMMAND} -E copy_directory
#		"${PROJECT_SOURCE_DIR}/dlls"
#		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
#endif()
