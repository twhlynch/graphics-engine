cmake_minimum_required(VERSION 3.10)
project(Renderer C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

include_directories(
	${OPENGL_INCLUDE_DIR}
	${glfw3_INCLUDE_DIR}
	${GLM_INCLUDE_DIRS}
	vendor/glad/include
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

target_link_libraries(Renderer
	glad_c
	${OPENGL_LIBRARIES}
	glfw
	glm::glm
)

add_custom_command(TARGET Renderer POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Renderer>/assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/assets
			$<TARGET_FILE_DIR:Renderer>/assets
)