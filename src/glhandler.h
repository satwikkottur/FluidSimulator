#ifndef GL_HANDLER
#define GL_HANDLER

#ifndef __APPLE__
    #include <GL/glew.h>
    #include <GL/glut.h>
#else
    //#include <GL/glew.h>
    #include <GLUT/glut.h>
#endif

#include "parameters.h"

//Function to handle re-sizing of window
void changeWindowSize(int, int);
//Function to render the scene
void renderScene();
//Handling keyboard events
void processNormalKeys(unsigned char, int, int);
 
#endif
