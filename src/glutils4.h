#ifndef OPENGL_FUNCS
#define OPENGL_FUNCS

#ifdef __APPLE__
    // Include GLEW
    #include <GL/glew.h>
    // Include GLUT
    #include <GLUT/glut.h>
    // Include GLFW
    #include <GLFW/glfw3.h>
#else
    #include <GL/glut.h>
#endif

#include <utility>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

// Geometry library for OpenGL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "parameters.h"

//Class to take care of all the hassles and programming syntaxes for opengl
class OpenGL{
    public:
        //Attribute 
        
        //Identifier for the various elements
        GLuint vertexArrayId,  // VBO object identifier
                vertexbuffer,  // Buffer for position of the vertices
                programId,     // Identifier for program with shaders
                matrixId;      // MVP matrix handler

    
        // Window for displaying
        GLFWwindow* window;

        // MVP matrix for controlling the view of the scene
        // Later keyboard binding added for different views
        glm::mat4 MVP;

        float scale; // Scale that changes by moving
        float scaleChange;
        
        float angle; // Angle that changes with change in viewpoint
        float angleChange; // Angle change 

        //Methods
    public:
        //Constructor
        OpenGL();
        
        //Initialization
        void initializeOpenGL(const char*, const char*);

        // Termination
        void terminateOpenGL();

        // Methods for reading, loading the shaders and creating program out of it
        // Code borrowed from : http://www.opengl-tutorial.org 
        // Stores the program id as an method which can be used for attaching
        void loadShaders(const char * vertex_file_path, const char * fragment_file_path);

        // Getting the transformation matrix using user interaction
        void registerUserInputs();

        // The main rendering part - empty for now
        // Takes in arguments of the current window and the program compiled by the shader
        void renderScene();
        
        // Dumping the window as image frames for offline rendering
        int windowDump();

        ///////////////////////////////////////////////////////////////////////////
        //Attributes and methods for debugging
        ///////////////////////////////////////////////////////////////////////////
        float spacing;
        float* vertexPos;

        void debugOpenGL(int, char**, GLuint);
};

#endif
