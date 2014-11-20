#include <fstream>
#include <utility>
#include <string>
#include <stdio.h>
#include <iostream>

//#include "GL/gl_cl.h"
#include "glutils4.h"
#include "timeMeasure.h"
#include "parameters.h"
#include "structs/structures.h"
#include "fluid.h"
#include "clutils.h"

//Global to hold the vertexBuffId from OpenGL and OpenCL
GLuint* vertexBuffId;
GLuint programId;
GLFWwindow* window;

//Fluid* water;
OpenGL* myGL;
OpenCL* myCL;
std::vector<cl::Memory> memVector;
cl::BufferGL* vertexBuffCL;
int iteration = 0;

void changeWindowSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv){
    //Initializing the time measure object
    TimeMeasure myTime;
    myTime.addTimePoint("Start");

    ////////////////////////////////////////////
    // Initializing and checking OpenCL part //
    //////////////////////////////////////////
    myCL = new OpenCL();
    myCL->initOpenCL();
  
    ////////////////////////////////////////////
    // Initializing and checking OpenGL part //
    //////////////////////////////////////////
    myGL = new OpenGL();
    myGL->initializeOpenGL();

    // Loading the shaders
    char vertexShaderPath[] = "shaders/SimpleVertexShader.v";
    char fragmentShaderPath[] = "shaders/SimpleFragmentShader.f";
    myGL->loadShaders(vertexShaderPath, fragmentShaderPath);
    
    // Looping around in a while
    do{
        // Using the current shaders
        myGL->renderScene();
    }
    while(glfwWindowShouldClose(myGL->window) == 0);

    myGL->terminateOpenGL();

    //myCL->initOpenCLwithOpenGL();
    
    //Initializing the particles
    //Fluid water;
    /*water = new Fluid();
    water->fluidCL = myCL;
    water->initParticles("BLOCK_IN_AIR");
    water->setupKernels();
    water->setupSystem();*/

    //water.runIteration();

    //Debugging variabes
    /*float spacing = 0.12;
    float* vertexPos = new float[3 * debugNo * debugNo * debugNo];

    int count = 0;
    for(int i = 0; i < debugNo; i++){
        for(int j = 0; j < debugNo; j++){
            for(int k = 0; k < debugNo; k++){
                vertexPos[3 * count] = (float)i * spacing;
                vertexPos[3 * count + 1] = (float)j * spacing;
                vertexPos[3 * count + 2] = (float)k * spacing;
                count++;
            }
        }
    }*/

    /*for(int i = 0; i < 1; i++){
        water->debug(vertexBuffCL);
        printf("Iteration main : %d\n", i);
        //myTime.addTimePoint("Iteration done");
    }*/
    
    //Initializing opengl
    //myGL.initOpenGL(argc, argv);

    //Initializing a vertex array and binding it to use it
    //glGenBuffers(1, &vertexBuffId);
    //glBindBuffer(GL_ARRAY_BUFFER, vertexBuffId);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * noParticles, water->particlePos, GL_DYNAMIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * noParticles, 0, GL_DYNAMIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 *debugNo*debugNo*debugNo, vertexPos, GL_DYNAMIC_DRAW);
    //Initializing the OpenCL and OpenGL common buffer
    /*vertexBuffCL = new BufferGL(*water->fluidCL->context, CL_MEM_WRITE_ONLY, vertexBuffId, NULL);
    memVector.push_back(*vertexBuffCL);

    //First iteration
    glFinish();
    water->fluidCL->queue->enqueueAcquireGLObjects(&memVector, NULL, NULL);
    water->debug(vertexBuffCL);
    water->fluidCL->queue->enqueueAcquireGLObjects(&memVector, NULL, NULL);*/

    //Setting up the shaders
	//myGL->setShaders();
    //GLUT main loop
	//glutMainLoop();
    //return 0;

    /*water.fluidCL = &myCL;
    water.initParticles("BLOCK_IN_AIR");
    myTime.addTimePoint("Particles initialized");
    water.transferDataToBuffer();
    myTime.addTimePoint("Buffer copied");
    water.setupKernels();
    water.setupSystem();
    myTime.addTimePoint("Kernels setup done");

    myTime.addTimePoint("Finished Iteration");
    //water.runIteration();
    for(int i = 0 ; i < 100; i++){
        water.debug();
        myTime.addTimePoint("Finished Iteration");
    }*/
    /*myTime.addTimePoint("Done running");
    myTime.printTimePoints();*/

    return 0;
}


