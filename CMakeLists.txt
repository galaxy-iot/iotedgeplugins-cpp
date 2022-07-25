cmake_minimum_required(VERSION 2.8.12)
project(iotedgeplugins)

# compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(./examples/generator)