cmake_minimum_required(VERSION 3.10)
project(Renderer C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_subdirectory(vendor/glfw)
target_compile_options(glfw PRIVATE -w)

include_directories(
	${OPENGL_INCLUDE_DIR}
	vendor/glad/include
	vendor/stb_image/include
	/usr/local/include
	/opt/homebrew/include
)

set_source_files_properties(vendor/glad/src/glad.c PROPERTIES LANGUAGE C)
add_library(glad_c STATIC vendor/glad/src/glad.c)

file(GLOB_RECURSE SOURCES
	src/*.cpp
	src/*.hpp
)

add_executable(Renderer
	${SOURCES}
)

target_compile_options(Renderer PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_link_libraries(Renderer
	${OPENGL_LIBRARIES}
	glfw
	glad_c
)

add_custom_target(assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/assets
		${CMAKE_BINARY_DIR}/assets
	COMMENT "Copying assets to the build directory"
)

add_dependencies(Renderer assets)
