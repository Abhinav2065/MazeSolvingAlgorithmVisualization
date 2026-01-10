#include <exception>
#include <queue>
#include <limits>
#include <functional>
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


enum MazeState {
    GENERATING,
    IDLE,
    SOLVING,
    SOLVED
};

class Maze {
    private:
        int width, height;
        std::vector<Cell> grid;

        // Variables for Genrating using DFS
        std::stack<std::pair<int, int>> stack;
        std::mt19937 rng;
        int currentX, currentY;

        // Variable for Dijkstra

        using Node = std::pair<int, int>;

        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

        std::vector<int> dist;
        std::vector<int> parentMap;
        std::vector<bool> solveVisited;
        std::vector<int> finalPath;

        MazeState state = IDLE;



        int index (int x, int y) const {
            if (x < 0 || x >= width || y < 0 || y >= height) {
                return -1;
            }
            return y * width + x;
        }

        


    public: 
        Maze(int w, int h): width(w), height(h) {
            grid.resize(width*height);
            rng.seed(std::time(0));
        }

        void setupGeneration() {
            grid.assign(width * height, Cell());
            
            while (!stack.empty()) stack.pop();

            currentX = 0;
            currentY = 0;
            grid[index(currentX, currentY)].visited = true;
            stack.push({currentX, currentY});

            finalPath.clear();

            state = GENERATING;
        }


        void step() {
            if (state == GENERATING) {
                stepGeneration();
            }
            else if (state == SOLVING) {
                stepDijkstra();
            }
        }
        
        
        void stepDijkstra() {
            if (pq.empty()) {
                state = SOLVED;
                return;
            }

            int currentCost = pq.top().first;
            int currentIdx = pq.top().second;
            pq.pop();
            

            if (currentCost > dist[currentIdx]) return;

            solveVisited[currentIdx] = true;

            if (currentIdx == index(width - 1, height -1)) {
                reconstructuredPath(currentIdx);  
                state = SOLVED;
                return;
            }


            int cx = currentIdx % width;
            int cy = currentIdx / width;

            int dx[] = {0,1,0,-1};
            int dy[] = {1,0,-1,0};

            for (int i = 0; i < 4; i++) {
                if (grid[currentIdx].walls[i]) continue;

                int nx = cx + dx[i];
                int ny = cy + dy[i];
                int nextIdx = index(nx, ny);

                if (nextIdx != -1) {
                    int newDist = dist[currentIdx] + 1;

                    if (newDist < dist[nextIdx]) {
                        dist[nextIdx] = newDist;
                        parentMap[nextIdx] = currentIdx;
                        pq.push({newDist, nextIdx});
                    }
                }
            }
        }

        void reconstructuredPath(int endIdx) {
            int curr = endIdx;
            while (curr != -1) {
                finalPath.push_back(curr);
                curr = parentMap[curr];
            }
        }


        void stepGeneration() {
            if (stack.empty()) {
                state = IDLE;
                return;
            }


            int x = currentX;
            int y = currentY;

            std::vector<int> neighours;


            if (y < height - 1 && !grid[index(x, y + 1)].visited) neighours.push_back(0);
            if (x < width - 1 && !grid[index(x + 1, y)].visited) neighours.push_back(1);
            if (y > 0 && !grid[index(x, y - 1)].visited) neighours.push_back(2);
            if (x > 0 && !grid[index(x - 1, y)].visited) neighours.push_back(3);

            if (!neighours.empty()) {
                
                // Pick up an random neigbor
                std::uniform_int_distribution<int> dist(0, neighours.size() -1);
                int dir = neighours[dist(rng)];

                stack.push({x,y});


                // remove walls
                if (dir == 0) { // Top
                    grid[index(x, y)].walls[0] = false;
                    currentY++;
                    grid[index(currentX, currentY)].walls[2] = false; 
                }

                else if (dir == 1) { // Right
                    grid[index(x, y)].walls[1] = false;
                    currentX++;
                    grid[index(currentX, currentY)].walls[3] = false; 
                }
                else if (dir == 2) { // Bottom
                    grid[index(x, y)].walls[2] = false;
                    currentY--;
                    grid[index(currentX, currentY)].walls[0] = false; 
                }
                else if (dir == 3) { // Left
                    grid[index(x, y)].walls[3] = false;
                    currentX--;
                    grid[index(currentX, currentY)].walls[1] = false; 
                }
                grid[index(currentX, currentY)].visited = true;
            }

            else {
                auto next = stack.top();
                stack.pop();
                currentX = next.first;
                currentY = next.second;
            
            }


        }

        void setupDijkstra() {
            if (state == GENERATING) return;

            // Rest the Data Structure
            while (!pq.empty()) pq.pop();
            finalPath.clear();

            int totalCells = width*height;
            dist.assign(totalCells, std::numeric_limits<int>::max()); // Infinite Distance
            parentMap.assign(totalCells, -1);
            solveVisited.assign(totalCells, false);


            // Start at (0,0)
            int startInx = index(0,0);
            dist[startInx] = 0;
            pq.push({0, startInx});

            state = SOLVING;
        }

        void render() {
            float cellSize = 2.0f / std::max(width, height);
            float offSetX = -1.0f + (2.0f - width * cellSize) / 2.0f;
            float offSetY = -1.0f + (2.0f - height * cellSize) / 2.0f;

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int idx = index(x,y);
                    Cell& cell = grid[idx];


                    float x1 = offSetX + x * cellSize;
                    float y1 = offSetY + y * cellSize;
                    float x2 = x1 + cellSize;
                    float y2 = y1 + cellSize;



                    // Draw Visited Backgrounds
                    if (cell.visited) {
                        glColor3f(0.2f, 0.6f, 0.2f);

                        if (state >= SOLVING && solveVisited[idx]) {
                            glColor3f(0.0f, 0.4f, 0.8f);
                        }

                        glBegin(GL_QUADS);
                        glVertex2f(x1, y1);
                        glVertex2f(x2, y1);
                        glVertex2f(x2, y2);
                        glVertex2f(x1, y2);
                        glEnd();
                    }


                // Render Walls
                glColor3f(1.0f, 1.0f, 1.0f);
                glBegin(GL_LINES);
                if (cell.walls[0]) {
                    glVertex2f(x1, y2);
                    glVertex2f(x2, y2);
                }
                if (cell.walls[1]) {
                    glVertex2f(x2, y1);
                    glVertex2f(x2, y2);
                }
                if (cell.walls[2]) {
                    glVertex2f(x1, y1);
                    glVertex2f(x2, y1);
                }
                if (cell.walls[3]) {
                    glVertex2f(x1, y1);
                    glVertex2f(x1, y2);
                }
            
                glEnd();
            

            // Make the starting Square (Green Square at the starting of the maze)
            float sx = offSetX;
            float sy = offSetY;
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(sx + 0.02f, sy + 0.02f);
            glVertex2f(sx + cellSize - 0.02f, sy + 0.02f);
            glVertex2f(sx + cellSize -0.02f, sy + cellSize - 0.02f);
            glVertex2f(sx + 0.02f, sy + cellSize - 0.02f);
            glEnd();


            // Make the ending Square

            glColor3f(1.0f, 0.0f, 0.0f);


            float ex = offSetX + (width - 1) * cellSize;
            float ey = offSetY + (height - 1) * cellSize;

            glBegin(GL_QUADS);
            glVertex2f(ex + 0.02f, ey + 0.02f);
            glVertex2f( ex + cellSize - 0.02f, ey + 0.02f);
            glVertex2f(ex + cellSize - 0.02f, ey + cellSize - 0.02f);
            glVertex2f(ex + 0.02f, ey + cellSize - 0.02f);
            glEnd();



            // Solution Rendering
            if (state == SOLVED && !finalPath.empty()) {
                glColor3f(1.0f, 0.8f, 0.0f);
                glLineWidth(4.0f);

                glBegin(GL_LINE_STRIP);
                for (int idx: finalPath) {
                    int px = idx % width;
                    int py = idx / width;


                    float cx = offSetX + px * cellSize + (cellSize / 2.0f);
                    float cy = offSetY + py * cellSize + (cellSize / 2.0f);

                    glVertex2f(cx, cy);
                }
                glEnd();

                glLineWidth(1.0f);
            }


        }

        
    }
        }


        bool isGenerationDone() {
            return state == IDLE;
        }

        void reset() {  
            setupGeneration();
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

    bool rKeyPressed = false;
    bool dKeyPressed = false;


    boxes.setupGeneration();

    // Render Loop ( This keeps the window open)

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }
        
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            if (!rKeyPressed) {
                boxes.reset();
                rKeyPressed = true;
            }

            else {
                rKeyPressed = false;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            if (!dKeyPressed) {
                if (boxes.isGenerationDone()) {
                    boxes.setupDijkstra();
                }
                dKeyPressed = true;
            }
            
            else {
                dKeyPressed = false;
            }
        }

        glClearColor(0.3f, 0.2f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        boxes.step();
        boxes.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}