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

#include "parameters.h"
#include "glhandler.h"

//Class to take care of all the hassles and programming syntaxes for opengl
class OpenGL{
    public:
        //Attribute 
        //Identifiers for program containing the shaders
        GLuint* programId = NULL;
        
        //Identifier for the vertex buffer
        GLuint vertexBufferId;
    
        // Window for displaying
        //GLFWwindow* window;

        //Methods
    public:
        //Constructor
        OpenGL();
        
        //Initialization
        GLFWwindow* initializeOpenGL();

        // Termination
        void terminateOpenGL();

        // Methods for reading, loading the shaders and creating program out of it
        // Code borrowed from : http://www.opengl-tutorial.org 
        // Stores the program id as an method which can be used for attaching
        GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);

        // The main rendering part - empty for now
        void renderScene(GLFWwindow*);
        
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