cmake_minimum_required(VERSION 3.12)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/bin)
include(Testing.cmake)

add_subdirectory(example/test1)
add_subdirectory(example/test2)
add_subdirectory(test-integrator)
add_subdirectory(test-integrator-validate)
add_subdirectory(test-materials)
add_subdirectory(test-camera)
add_subdirectory(test-sampler)
add_subdirectory(test-triangle)
add_subdirectory(test-bvh)