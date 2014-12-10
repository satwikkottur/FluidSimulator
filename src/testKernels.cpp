// List of functions to test the right functionalities of the kernel
#include "fluid.h"

// Each kernel will have a test function
void Fluid::testPositionCopyKernel(){
    int testNo = 10;
    // Copying from the particle buffer to see if its copied properly
    particles = fluidCL->readFromBuffer<struct Particle>(partBuff, testNo);

    // Printing the position from the particle 
    //printParticleInfo(0);
    particlePos = fluidCL->readFromBuffer<float>(particlePosBuff, 3*testNo);

    // Printing
    printf("====================Printing first few element===================\n");
    for(int i = 0; i < testNo; i++){
        printf("%f %f %f\n", particlePos[3*i+0], particlePos[1], particlePos[2]);
        printf("%f %f %f\n", particles[i].pos.x, particles[i].pos.y, particles[i].pos.z);
        printf("=================================================================\n");
    }

    printf("\n\n");
}

void Fluid::testExternalForceKernel(){
    int testNo = 10;
    // Copying from the particle buffer to see if its copied properly
    particles = fluidCL->readFromBuffer<struct Particle>(partBuff, testNo);

    // Printing the position from the particle 
    //printParticleInfo(0);
    //particlePos = fluidCL->readFromBuffer<float>(particlePosBuff, 3*testNo);

    // Printing
    printf("====================Printing first few element===================\n");
    for(int i = 0; i < testNo; i++){
        printParticleInfo(i);
        //printf("%f %f %f\n", particlePos[3*i+0], particlePos[1], particlePos[2]);
        //printf("%f %f %f\n", particles[i].pos.x, particles[i].pos.y, particles[i].pos.z);
        printf("=================================================================\n");
    }

    printf("\n\n");
}
