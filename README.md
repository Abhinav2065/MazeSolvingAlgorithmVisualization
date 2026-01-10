## Maze Solving Algorithm Visualization


This is a simple maze solving algorithm visualization which vizualizes different maze solving algorithms using OpenGL with C++.

This program is ment to show the usecase, effectiveness and usefullness of different maze solving algorithms in different cases.


# Usage

After running this program, it will use Depth First Search (DFS) to create a maze. You can see the maze creation as you open the program, after the maze is generated you can press the "R" key to regenerate the maze.

After the maze is generated you can press the "D" key to solve that maze using Dijkstra's algorithm. You can see the solution in real time as well, the blue color backgrounds are the cells visited by the solving algorithm and once it finds a solution then it will mark that soltution with an yellow line. 


# Prerequisites

You need a C++ Compiler and following libraries installed on your system:

1. GLFW3
2. GLEW
3. GLM
4. CMake


Linux / MacOS:

mkdir build && cd build
cmake ..
make
./MazeSolver


Windows:

1. Install dependency: vcpkg install glew glfw3 glm

Open in Visual Studio and let it confugure CMake Automatically. 

