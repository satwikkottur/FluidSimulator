// List of functions to run the kernels
#include "fluid.h"

// Each kernel will have a run function
void Fluid::runPositionCopyKernel(){
    //Work groups initialization
    NDRange global(_N);
    NDRange local(1);

    // Assigning the arguments
    posCopyKernel->setArg(0, *partBuff);
    posCopyKernel->setArg(1, *particlePosBuff);
    
    // Running the kernel
    fluidCL->runKernel(posCopyKernel, NullRange, global, local);
}

void Fluid::runExternalForceKernel(){
    // Work groups initialization
    NDRange global(_N);
    NDRange local(1);
    
    // Assinging the arguments
    externalForceKernel->setArg(0, *partBuff);
    externalForceKernel->setArg(1, *accBuff);
    externalForceKernel->setArg(2, *timeBuff);
    externalForceKernel->setArg(3, *voxelInfoBuff);

    // Running the kernel
    fluidCL->runKernel(externalForceKernel, NullRange, global, local);
}

