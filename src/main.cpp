#include <fstream>
#include <utility>
#include <string>
#include <stdio.h>
#include <iostream>

// Adding the geometry handling library for OpenGL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "GL/gl_cl.h"
#include "glutils4.h"
#include "timeMeasure.h"
#include "parameters.h"
#include "structs/structures.h"
#include "fluid.h"
#include "clutils.h"

//Global to hold the vertexBuffId from OpenGL and OpenCL
GLuint* vertexBuffId;

//Fluid* water;
OpenGL* myGL;
OpenCL* myCL;
std::vector<cl::Memory> memVector;
cl::BufferGL* vertexBuffCL;
int iteration = 0;

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
    // Initializing the particles in air //
    //////////////////////////////////////////
    Fluid* water;
    water = new Fluid();
    water->fluidCL = myCL;
    water->initParticles("BLOCK_IN_AIR");
    water->setupKernels();
    water->setupSystem();

    //water->runIteration();
    if(0){
          ///////////////////////////////////////////
         // Initializing and checking OpenGL part //
        ///////////////////////////////////////////
        myGL = new OpenGL();
        
        // Loading the shader paths
        char vertexShaderPath[] = "shaders/SphereVertexShader.v";
        char fragmentShaderPath[] = "shaders/SphereFragmentShader.f";
        myGL->initializeOpenGL(vertexShaderPath, fragmentShaderPath);
        
        // Looping around in a while
        do{
            // Rendering the scene
            myGL->renderScene();
        }
        while(glfwWindowShouldClose(myGL->window) == 0 &&
            glfwGetKey(myGL->window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

        // Cleaning up objects, VBOs, vertex arrays
        myGL->terminateOpenGL();
    }

    //myCL->initOpenCLwithOpenGL();
    
    //Initializing the particles
    //water.runIteration();

    for(int i = 0; i < 1; i++){
        water->debug(vertexBuffCL);
        printf("Iteration main : %d\n", i);
        //myTime.addTimePoint("Iteration done");
    }
    
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
