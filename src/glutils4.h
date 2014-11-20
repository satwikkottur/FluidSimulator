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

#include "parameters.h"
#include "glhandler.h"


//Class to take care of all the hassles and programming syntaxes for opengl
class OpenGL{
    public:
        //Attribute 
        //Identifiers for shaders and program
        GLuint vertShaderId, fragShaderId, programId;
        
        //Identifier for the vertex buffer
        GLuint vertexBufferId;
    
        // Window for displaying
        GLFWwindow* window;

        //Methods
    public:
        //Constructor
        OpenGL();
        //Initialization
        void initOpenGL();

        //Error printing functions for shaders, programs and other related events
        int printOglError(char*, int);
        void printShaderInfoLog(GLuint);
        void printProgramInfoLog(GLuint);
        
        //Method to set the shaders
        void setShaders();

        //Dumping the window
        int windowDump();

        //Read and writing text files
        char *textFileRead(char*);
        int textFileWrite(char*, char*);

        ///////////////////////////////////////////////////////////////////////////
        //Attributes and methods for debugging
        ///////////////////////////////////////////////////////////////////////////
        float spacing;
        float* vertexPos;

        void debugOpenGL(int, char**, GLuint);
};

#endif
