cmake_minimum_required(VERSION 3.12)
project(client)

add_definitions(-std=c++17 -O3)
include_directories(include)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADDERS src/*.hpp)

set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-variable") #-Wextra
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
#find_package(glm REQUIRED)

add_subdirectory(lib/glm EXCLUDE_FROM_ALL)

# Create an executable target
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link GLFW and OpenGL libraries to your target
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL GLEW::GLEW glm::glm)



