set(targetName "Test_Setup")

# External libraries
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Threads REQUIRED)
find_package(X11 REQUIRED COMPONENTS Xxf86vm Xi Xrandr)
# TODO: Check if included for each individual package


# Files
set(testSetupFiles 
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)


# Create library
add_executable(${targetName} ${testSetupFiles})

target_link_libraries(${targetName} PRIVATE       # Link external libraries
    glfw
    vulkan
    ${CMAKE_THREAD_LIBS_INIT}
    X11 Xxf86vm Xi Xrandr
)
set_target_properties(${targetName} PROPERTIES FOLDER "${ideFolderTests}")  # Set project location in solution tree


# Setup project settings
set_project_warnings(${targetName})  # Which warnings to enable
set_compile_options(${targetName})   # Which extra compiler flags to enable
set_output_directory(${targetName})  # Set the output directory of the library
