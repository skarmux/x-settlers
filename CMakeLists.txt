cmake_minimum_required(VERSION 3.10.2)

# set the project name
project(X-Settlers VERSION 0.1)

if(ANDROID)
	# build native_app_glue as a static lib
	set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
	add_library(native_app_glue STATIC
		${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
		
	# now build app's shared lib
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")	
		
	# Export ANativeActivity_onCreate(),
	# Refer to: https://github.com/android-ndk/ndk/issues/381.
	set(CMAKE_SHARED_LINKER_FLAGS
		"${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
	
	add_library(native-activity SHARED main.cpp)
	
	target_include_directories(native-activity PRIVATE
		${ANDROID_NDK}/sources/android/native_app_glue)
		
	# add lib dependencies
	target_link_libraries(native-activity
		android
		native_app_glue
		EGL
		GLESv1_CM
		log)
else()

	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED True)

	# add source files
	file(GLOB_RECURSE SOURCE_FILES
			"src/*.cpp"
			"src/*.h"
			)

	# Main executable
	add_executable(${PROJECT_NAME} ${SOURCE_FILES})
	target_include_directories(${PROJECT_NAME} PRIVATE
			"${CMAKE_SOURCE_DIR}/src"
			"${CMAKE_SOURCE_DIR}"
			"${CMAKE_SOURCE_DIR}/third_party/glm"
			"${CMAKE_SOURCE_DIR}/third_party/SDL2/include"
			"${CMAKE_SOURCE_DIR}/third_party/spdlog/include"
			"${CMAKE_SOURCE_DIR}/third_party/stb"
			)
	if(WIN32)
		#target_link_libraries(${PROJECT_NAME} mingw32) # Must be before SDL
	endif()

	# Platform
	set(PLATFORM_DEFAULT "SDL")
	set(PLATFORM_TYPES "SDL" "Android")
	set(PLATFORM ${PLATFORM_DEFAULT} CACHE STRING "Project platform, default: ${PLATFORM_DEFAULT}")
	set_property(CACHE PLATFORM PROPERTY STRINGS ${PLATFORM_TYPES})
	# TODO: make actual use of it

	include_directories(
			"${CMAKE_SOURCE_DIR}/src"
			"${CMAKE_SOURCE_DIR}/third_party/SDL2/include"
			"${CMAKE_SOURCE_DIR}/third_party/glm"
			"${CMAKE_SOURCE_DIR}/third_party/glad/include"
			"${CMAKE_SOURCE_DIR}/third_party/spdlog/include"
			"${CMAKE_SOURCE_DIR}/third_party/stb")

	# SDL - assume that user has it installed in OS
	#find_package(SDL2 REQUIRED)
	#include_directories(${SDL2_INCLUDE_DIRS})
	include_directories("${CMAKE_SOURCE_DIR}/third_party/SDL2/include")
	link_directories("${CMAKE_SOURCE_DIR}/third_party/SDL2/lib/x64")
	if(WIN32)
		target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
	else()
		target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
	endif()

	# Renderer
	set(RENDERER_DEFAULT "OpenGL")
	set(RENDERER_TYPES "OpenGL" "Vulkan")
	set(RENDERER ${RENDERER_DEFAULT} CACHE STRING "Project renderer, default: ${RENDERER_DEFAULT}")
	set_property(CACHE RENDERER PROPERTY STRINGS ${RENDERER_TYPES})

	message(STATUS "Using '${RENDERER}' renderer")
	if(RENDERER MATCHES Vulkan)
		find_package(Vulkan REQUIRED)
		# TODO: target_link_libraries(something)
		target_compile_definitions(${PROJECT_NAME} PRIVATE RENDERER_VULKAN)
	elseif(RENDERER MATCHES OpenGL)
		find_package(OpenGL REQUIRED)
		add_subdirectory("third_party/glad")
		target_link_libraries(${PROJECT_NAME} glad)
		target_compile_definitions(${PROJECT_NAME} PRIVATE RENDERER_OPENGL)
	else()
		message(FATAL_ERROR "Couldn't deduce renderer! Please install OpenGL or Vulkan")
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

		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	endif()

	# copy dlls
	#if(WIN32)
	#	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	#		COMMAND ${CMAKE_COMMAND} -E copy_directory
	#		"${PROJECT_SOURCE_DIR}/dlls"
	#		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	#endif()

endif()