// Core OpenGL Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <climits>
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>



int main() {


    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    // Initilize GLFW
    if (!glfwInit()){
        std::cerr << "Failed to initilize GLFW\n";
        return -1;
    }


    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11); // Force to use X11 cuz its not working on wayland for me :(

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create the Window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Starter Project", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to Create an GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);



    return 0;
}