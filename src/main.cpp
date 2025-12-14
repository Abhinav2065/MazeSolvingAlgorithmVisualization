// Core OpenGL Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <climits>
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <ostream>



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
    GLFWwindow* window = glfwCreateWindow(1200, 800, "OpenGL Starter Project", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to Create an GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to Initilize GLEW " << std::endl;
        return -1;
    }


    // Define viewport Dimentions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Render Loop ( This keeps the window open)

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }


        glClearColor(0.3f, 0.2f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}