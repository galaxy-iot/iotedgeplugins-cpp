cmake_minimum_required(VERSION 2.8.12)
project(generator)

include_directories(${CMAKE_SOURCE_DIR}/protobuf/function)
aux_source_directory(${CMAKE_SOURCE_DIR}/protobuf/function function_srcs)

add_library(function STATIC
  ${function_srcs}
)
target_link_libraries(function ${CONAN_LIBS})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src generator_srcs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(generator 
    ${generator_srcs}
)

target_link_libraries(generator 
    function
)