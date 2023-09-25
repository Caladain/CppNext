cmake_minimum_required(VERSION 3.24)

project(CppNext CXX)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT cppn_compiler)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(fmt CONFIG REQUIRED)
find_package(Tracy CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(ctre CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>) #Multiprocessor Compilation
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/WX>) #Warnings as Errors
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4>) #Enable warnings level 4.

add_subdirectory(compiler)