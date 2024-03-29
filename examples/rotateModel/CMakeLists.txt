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

target_include_directories(${targetName} PRIVATE  # Reference lib folder
    ${CMAKE_BINARY_DIR}/out/include
)
target_link_libraries(${targetName} PRIVATE "lwEngine")
set_target_properties(${targetName} PROPERTIES FOLDER "${ideFolderExamples}")  # Set project location in solution tree

# Setup project settings
set_project_warnings(${targetName})  # Which warnings to enable
set_compile_options(${targetName})   # Which extra compiler flags to enable
set_output_directory(${targetName})  # Set the output directory of the library


# Add resource path macros
target_compile_definitions(${targetName} PRIVATE RESOURCE_PATH_VIKING_MODEL="${CMAKE_CURRENT_SOURCE_DIR}/resources/models/viking_room.obj")
target_compile_definitions(${targetName} PRIVATE RESOURCE_PATH_VIKING_TEXTURE="${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/viking_room.png")

target_compile_definitions(${targetName} PRIVATE SHADER_PATH_VERTEX="${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/vert.spv")
target_compile_definitions(${targetName} PRIVATE SHADER_PATH_FRAGMENT="${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/frag.spv")

execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/compile.sh")