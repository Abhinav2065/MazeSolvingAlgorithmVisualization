// Core OpenGL Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <asm-generic/errno.h>
#include <climits>
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <random>
#include <ctime>


struct Cell {
    bool visited = false;
    bool walls[4] = {true, true, true, true}; // For each direction in a 2d space.
};

class Maze {
    private:
        int width, height;
        std::vector<Cell> grid;
        std::stack<std::pair<int, int>> stack;
        std::mt19937 rng;

        int index (int x, int y) const {
            return y * width + x;
        }


    public: 
        Maze(int w, int h): width(w), height(h) {
            grid.resize(width*height);
        }

        void render() {
            float cellSize = 2.0f / std::max(width,height);
            float offSetX = -1.0f + (2.0f - width*cellSize)/2.0f;
            float offSetY = -1.0f + (2.0f - height*cellSize) / 2.0f;


            // Render Cells

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    Cell& cell = grid[index(x,y)];

                    // Center Position of the Cell
                    float cx = offSetX + x * cellSize + cellSize / 2;
                    float cy = offSetY + y * cellSize + cellSize / 2;

                    // Draw Box Colors
                    glColor3f(0.8f, 0.8f, 0.8f);
                    glBegin(GL_QUADS);
                    glVertex2f(cx - cellSize/2, cy - cellSize/2);
                    glVertex2f(cx + cellSize/2, cy - cellSize/2);
                    glVertex2f(cx + cellSize/2, cy + cellSize/2);
                    glVertex2f(cx - cellSize/2, cy + cellSize/2);
                    glEnd();


                    // Draw Box Borders
                    glColor3f(0.0f, 0.0f, 0.0f);
                    glLineWidth(2.0f);
                    glBegin(GL_LINE_LOOP);
                    glVertex2f(cx - cellSize/2, cy - cellSize/2);
                    glVertex2f(cx + cellSize/2, cy - cellSize/2);
                    glVertex2f(cx + cellSize/2, cy + cellSize/2);
                    glVertex2f(cx - cellSize/2, cy + cellSize/2);
                    glEnd();
                }
            }
        }
        
};


// Shader sources
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)";


GLuint createShaderProgram() {
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}



int main() {


    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    // Initilize GLFW
    if (!glfwInit()){
        std::cerr << "Failed to initilize GLFW\n";
        return -1;
    }


    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11); // Force to use X11 cuz its not working on wayland for me :(


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    
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

    Maze boxes(12,12);


    // Render Loop ( This keeps the window open)

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }


        glClearColor(0.3f, 0.2f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        boxes.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}