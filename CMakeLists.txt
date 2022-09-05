cmake_minimum_required(VERSION 2.8.12)
project(iotedgeplugins)

# compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# include protobuf files
include_directories(${CMAKE_SOURCE_DIR}/protobuf/function)
aux_source_directory(${CMAKE_SOURCE_DIR}/protobuf/function function_srcs)

add_library(function STATIC
  ${function_srcs}
)

target_link_libraries(function ${CONAN_LIBS})

include_directories(${CMAKE_SOURCE_DIR}/common/include)

add_subdirectory(./examples/generator)
add_subdirectory(./examples/device_drop_processor)
add_subdirectory(./examples/device_event_processor)