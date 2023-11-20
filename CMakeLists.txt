cmake_minimum_required(VERSION 3.22)
project(semSistemasOperativos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -pedantic -Wextra)

function(add_program TARGET_NAME)
	file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}/src/*.cpp")
	add_executable(${TARGET_NAME} ${SOURCES})
	target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}/include")
endfunction()

add_program(05_generador)
