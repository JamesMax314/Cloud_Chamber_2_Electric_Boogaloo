project (client)
cmake_minimum_required(VERSION 3.5.1)
option(JS_ONLY "Compiles to native JS (No WASM)" OFF)

add_definitions(-std=c++17 -O3)
include_directories(include)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADDERS src/*.hpp)


# find_package(OpenGL REQUIRED)

file(COPY "${CMAKE_SOURCE_DIR}/ui/" DESTINATION "${CMAKE_SOURCE_DIR}/public")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/public")
add_executable(a ${HEADDERS} ${SOURCES})

if(JS_ONLY)
	message(STATUS "Setting compilation target to native JavaScript")
	set(CMAKE_EXECUTABLE_SUFFIX ".js")
	set_target_properties(a PROPERTIES LINK_FLAGS "-s WASM=0 -s EXPORTED_FUNCTIONS='[_main]' --bind -s USE_GLFW=3 -s USE_WEBGL2=1")
else(JS_ONLY)
	message(STATUS "Setting compilation target to WASM")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set_target_properties(a PROPERTIES LINK_FLAGS "-s WASM=3 -s EXPORTED_FUNCTIONS='[_main]' --bind -s USE_GLFW=3 -s USE_WEBGL2=1")
endif(JS_ONLY)