cmake_minimum_required(VERSION 3.10)

# set the project name
project(X-Settlers VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add source files
file(GLOB_RECURSE SOURCE_FILES
	"${CMAKE_SOURCE_DIR}/src/*.c"
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/*.h"
	"${CMAKE_SOURCE_DIR}/src/*.hpp")

link_directories(
	"${CMAKE_SOURCE_DIR}/thirdparty/SDL2/lib/x64")

include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/thirdparty/SDL2/include"
	"${CMAKE_SOURCE_DIR}/thirdparty/glm"
	"${CMAKE_SOURCE_DIR}/thirdparty/glad/include"
	"${CMAKE_SOURCE_DIR}/thirdparty/spdlog/include"
	"${CMAKE_SOURCE_DIR}/thirdparty/stb")

# dependencies
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

if(WIN32)
	set(LIBS SDL opengl32 GLAD)
elseif(UNIX)
	set(LIBS SDL GL GLAD X11 Xrandr Xi dl pthread)
endif()

#add_executable("${PROJECT_NAME}" "X-Settlers.cpp")
	
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