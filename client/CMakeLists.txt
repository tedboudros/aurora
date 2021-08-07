cmake_minimum_required(VERSION 3.8)
project(Aurora)

if(WIN32 AND NOT MINGW)
	message(FATAL_ERROR "MinGW is required for building the project.")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

file(GLOB_RECURSE SRC_LIST src/*.cpp src/*.c include/*.hpp include/*.h)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE "include")

# For windows networking
IF(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE -lwsock32 -lWs2_32)
endif()

if(MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE -lmingw32)
endif()
# TODO: Couldn't find some portable way of setting this.
#       If CMake will add something in future, replace this.
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

# Using C11 for now, seems to be working (before CMake there was C17).
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
# Fixes WinMain bug
add_definitions(-DSDL_MAIN_HANDLED)

# SDL2_image doesn't have a script for find_package(), so using find_library().
find_library(SDL2_IMAGE_LIB SDL2_image)
if(NOT SDL2_IMAGE_LIB)
	message(FATAL_ERROR "SDL2_image library is required, but not found.")
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_LIB})

# Same for SDL2_ttf.
find_library(SDL2_TTF_LIB SDL2_ttf)
if(NOT SDL2_TTF_LIB)
	message(FATAL_ERROR "SDL2_ttf library is required, but not found.")
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_TTF_LIB})


# Install (copy files to aurora/bin/<Config>)

set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_INSTALL_PREFIX "${BIN_DIR}/${CMAKE_BUILD_TYPE}")
install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/res DESTINATION .)
if(WIN32)
    file(GLOB STATIC_WIN_FILES ${PROJECT_SOURCE_DIR}/static/windows/*)
    install(FILES ${STATIC_WIN_FILES} DESTINATION .)
endif()


# Zip 

set(ZIP_FILE_PATH "${BIN_DIR}/${PROJECT_NAME}-${CMAKE_BUILD_TYPE}.zip")
add_custom_target(make_zip
				  COMMAND ${CMAKE_COMMAND} -E echo "Creating zip-file: ${ZIP_FILE_PATH}"
                  COMMAND ${CMAKE_COMMAND} -E tar "cfv" ${ZIP_FILE_PATH} --format=zip .
				  WORKING_DIRECTORY "${BIN_DIR}/$<CONFIG>"
				 )
