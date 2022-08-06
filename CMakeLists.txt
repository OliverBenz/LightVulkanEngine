cmake_minimum_required(VERSION 3.12)

project("LightVulkanEngine")
set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build the unit tests for the project." True)
option(BUILD_EXAMPLES "Build examples for the project." True)

# Setup project settings
include(cmake/ProjectSettings.cmake)
include(cmake/CompilerWarnings.cmake)


add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()