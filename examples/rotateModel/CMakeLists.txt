set(targetName "RotatingModel")

set(executableFiles
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/renderSystem.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/renderSystem.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/application.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/application.hpp"
)


# Create library
add_executable(${targetName} ${executableFiles})

# TODO: Use the include dir from output directory so the includes are in the format #include "lwEngine/..."
target_include_directories(${targetName} PRIVATE  # Reference lib folder
    ${CMAKE_SOURCE_DIR}/src/core
)
target_link_libraries(${targetName} PRIVATE "lwEngine")


# Options
set_compile_options(${targetName})   # Which extra compiler flags to enable