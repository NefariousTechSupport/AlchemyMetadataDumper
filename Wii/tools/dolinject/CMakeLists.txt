cmake_minimum_required(VERSION 3.13)

project(dolinject LANGUAGES CXX VERSION 1.0.0)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/source/ComputeBases.cpp
	${CMAKE_SOURCE_DIR}/source/dol.cpp
	${CMAKE_SOURCE_DIR}/source/dol.h
	${CMAKE_SOURCE_DIR}/source/Inject.cpp
	${CMAKE_SOURCE_DIR}/source/main.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Wextra -Werror)

target_include_directories(${PROJECT_NAME} PRIVATE
	source
)
