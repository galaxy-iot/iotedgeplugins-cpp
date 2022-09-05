cmake_minimum_required(VERSION 2.8.12)
project(device_event_processor)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src generator_srcs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(device_event_processor 
    ${generator_srcs}
)

target_link_libraries(device_event_processor 
    function
)