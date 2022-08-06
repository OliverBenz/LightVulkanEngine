# Settings
include(../cmake/CustomCommands.cmake)
set(targetName "lwEngine")


# External libraries
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Threads REQUIRED)
find_package(X11 REQUIRED COMPONENTS Xxf86vm Xi Xrandr)
# TODO: Check if included for each individual package


# Get project files
include("core/Files.cmake")

# Create library
add_library(${targetName}
    # Core
    ${coreHeaders}
    ${coreSources}

    # RenderSystems  TODO: Move outside from core library
    ${renderSystemsHeaders}
    ${renderSystemsSources}
)

# Link libraries.
target_include_directories(${targetName} PRIVATE  # Reference lib folder
    ${CMAKE_SOURCE_DIR}/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)
target_link_libraries(${targetName} PRIVATE       # Link external libraries
    glfw
    vulkan
    ${CMAKE_THREAD_LIBS_INIT}
    X11 Xxf86vm Xi Xrandr
)
set_target_properties(${targetName} PROPERTIES FOLDER "${ideFolderCore}")  # Set project location in solution tree

# Setup project settings
# set_project_warnings(${targetName})  # Which warnings to enable
set_compile_options(${targetName})   # Which extra compiler flags to enable
set_output_directory(${targetName})  # Set the output directory of the library

# Copy header files to output after build
copy_headers_to_output(${targetName} "${coreHeaders}")

# Specify version
target_compile_definitions(${targetName} PUBLIC VERSION_MAJOR=1)
target_compile_definitions(${targetName} PUBLIC VERSION_MINOR=1)
target_compile_definitions(${targetName} PUBLIC VERSION_PATCH=2)

# System installation
install(TARGETS ${targetName} DESTINATION lib)
install(FILES ${coreHeaders} DESTINATION include/${targetName})
