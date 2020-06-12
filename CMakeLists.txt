cmake_minimum_required(VERSION 3.10)

# set the solution name
project(X_SETTLERS)

# add thrid party libraries
add_subdirectory(third_party)

if(WIN32)
    # use precompiled SDL2 binaries
    include_directories(${CMAKE_SOURCE_DIR}/third_party/SDL2/include)
    link_directories(${CMAKE_SOURCE_DIR}/third_party/SDL2/lib/x64)
endif()

# add engine
add_subdirectory(engine)

# add game
add_subdirectory(xsettlers)

#target_link_libraries(xsettlers PUBLIC engine)

if(ANDROID)
	# add xsettlers app
    add_subdirectory(app)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT xsettlers)
