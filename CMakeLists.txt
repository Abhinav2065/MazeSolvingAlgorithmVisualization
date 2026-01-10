cmake_minimum_required(VERSION 3.10)
project(MazeSolver)

set(CMAKE_CXX_STANDARD 17)

# 1. Find Packages (Works on all OS)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# 2. Define the Executable
add_executable(MazeSolver src/main.cpp)

# 3. Link Libraries
target_link_libraries(MazeSolver
    PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
)

# 4. macOS Specific Fixes (CMake handles the frameworks automatically here!)
if(APPLE)
    target_link_libraries(MazeSolver PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()