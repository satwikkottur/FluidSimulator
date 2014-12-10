#ifndef FLUID
#define FLUID

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <utility>
#include <cmath>
#ifdef __APPLE__
    #include <OpenCL/cl.hpp>
#elif
    #include "CL/cl.hpp"
#endif
#include "clutils.h"
#include "structs/structures.h"
#include "parameters.h"

class Fluid{
    public:
        //Attributes
        int _N;
        float _t;
        //OpenCL object
        OpenCL* fluidCL;
        //
    //private:
    public:
        //Private attributes
        //Particles
        struct Particle* particles;
        Buffer* partBuff;
        //Buffer for histogram
        Buffer* histBuff;
        //Buffer for holding the power of digit
        Buffer* digitBuff;
        /////////////////////////////////////////////////////////////////////////
        //Radix algorithm
        Kernel* histKernel;
        Kernel* partKernel;
        Kernel* copyKernel;
        Kernel* resetKernel;
        struct Histogram* hists;
        struct Histogram* startPts;
        struct Particle* sortParticles;
        int* digitPower;
        Buffer* startPtBuff;
        Buffer* sortPartBuff;
        Buffer* tempBuff;
        //////////////////////////////////////////////////////////////////
        //Updating step
        Kernel* externalForceKernel;
        struct Tuple* acceleration;
        struct VoxelHistogram* voxelHist;
        struct ParticleHistogram* partHist;
        float* timeStep;
        int* hashParams;
        Buffer* accBuff;
        Buffer* timeBuff;
        Buffer* hashParamBuff;
        Buffer* voxelHistBuff;
        Buffer* voxelIndexBuff;
        Buffer* restDensityBuff;
        Buffer* partHistBuff;

        //Voxel hashing
        Kernel* voxelKernel;
        Kernel* densityKernel;
        Kernel* hashKernel;

        //Particle hashing
        Kernel* partHashKernel;
        //Buffer containing information regarding number of particles and voxel
        int* voxelInfo;
        Buffer* voxelInfoBuff;
        //Kernel to estimate the value of lambda
        Kernel* lambdaKernel;
        //Kernel to estimate the neighbours
        Kernel* nbrKernel;
        struct ParticleNbrs* ptNbrs;
        Buffer* ptNbrBuff;
        //Kernel to estimate the displacement change
        Kernel* displacementKernel;
        //Kernel to copy the position values to float array
        Kernel* posCopyKernel;
        //Kernel to update position and velocity
        Kernel* velocityKernel;
        //Kernel to handle collisions
        Kernel* collisionKernel;
        //Float array to hold the vertex positions for OpenGL rendering
        float* particlePos;
        Buffer* particlePosBuff;
        Buffer* sortParticlePosBuff;
        //Methods
    public:
        //Constructor
        Fluid();
        //Initialization
        void initParticles(std::string initState);
        //Copying data
        void transferDataToBuffer();
        //Setting up the kernels
        void setupKernels();
        //Setting up buffers, etc
        void setupSystem();
        //Run an iteration
        void runIteration();
        //Radix on CPU
        void radixCPU();
        //Radix on GPU
        void radixGPU();
        //Debug(Currently the main function)
        void debug(BufferGL*);
        //Finding the density of the fluid using current positions
        float findRestDensity();
        //Printing the particle information for debugging
        void printParticleInfo(int);

        // Running kernel methods
        void runPositionCopyKernel();
        void runExternalForceKernel();

        // Testing kernel methods
        void testPositionCopyKernel();
        void testExternalForceKernel();

    private:
        //Attributes
        //Buffer for the particles
        //////////////////////////////////////////////////////////////////
        //Methods
};

#endif
