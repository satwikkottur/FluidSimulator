#include <fstream>
#include <utility>
#include <string>
#include <stdio.h>
#include <iostream>

//#include "GL/gl_cl.h"
#include "glutils.h"
#include "timeMeasure.h"
#include "parameters.h"
#include "structs/structures.h"
//#include "fluid.h"
#include "clutils.h"

//Global to hold the vertexBuffId from OpenGL and OpenCL
GLuint* vertexBuffId;
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

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Rotation
    glRotatef(0.5, 0, 1, 0);
	glRotatef(0.1, 0, 1, 0);
    
    //Current Iteration
    /*glFinish();
    //water->fluidCL->queue->enqueueAcquireGLObjects(&memVector, NULL, NULL);
    water->debug(vertexBuffCL);
    //water->fluidCL->queue->enqueueAcquireGLObjects(&memVector, NULL, NULL);
    
    //Debug
    //water->printParticleInfo(0);
    printf("Iteration : %d\n", iteration);
    iteration++;
    if(iteration > 1000) exit(1);

    //activate and specify pointer to vertex array
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * noParticles, water->particlePos, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffId);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(0.2, 0.2, 0.9);
    //glDrawArrays(GL_POINTS, 0, debugNo*debugNo*debugNo);
    glDrawArrays(GL_POINTS, 0, noParticles);
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);*/

	glutSwapBuffers();
    glutPostRedisplay();
    //myGL->windowDump();    
}

void processNormalKeys(unsigned char key, int x, int y) {
    switch(key){
        case 27:
            exit(0);
    }
}

int main(int argc, char** argv){
    //Initializing the time measure object
    TimeMeasure myTime;
    myTime.addTimePoint("Start");

    //Intializing the opencl and opengl parts
    myCL = new OpenCL();
    //myGL = new OpenGL();
    myCL->initOpenCL();
    //myGL->initOpenGL(argc, argv);
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
