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
    
    // Assigning the arguments
    externalForceKernel->setArg(0, *partBuff);
    externalForceKernel->setArg(1, *accBuff);
    externalForceKernel->setArg(2, *timeBuff);
    externalForceKernel->setArg(3, *voxelInfoBuff);

    // Running the kernel
    fluidCL->runKernel(externalForceKernel, NullRange, global, local);
}

void Fluid::runHashKernel(){
    // Work groups initialization
    NDRange global(_N);
    NDRange local(1);

    // Assigning the arguments
    partHashKernel->setArg(0, *partHistBuff);
    partHashKernel->setArg(1, *partBuff);
    partHashKernel->setArg(2, *voxelInfoBuff);

    // Executing the kernel
    fluidCL->runKernel(partHashKernel, NullRange, local, local);
}

void Fluid::runNeighborKernel(){
    // Work groups initialization
    NDRange global(_N);
    NDRange local(1);

    // Setting the arguments of the kernel
    nbrKernel->setArg(0, *ptNbrBuff);
    nbrKernel->setArg(1, *partBuff);
    nbrKernel->setArg(2, *partHistBuff);
    nbrKernel->setArg(3, *voxelInfoBuff);

    // Executing the kernel
    fluidCL->runKernel(nbrKernel, NullRange, global, local);
}

void Fluid::runDensityEstimationKernel(){
    // Work groups initialization
    NDRange global(_N);
    NDRange local(1);

    // Setting the arguments of the kernel
    densityKernel->setArg(0, *ptNbrBuff);
    densityKernel->setArg(1, *partBuff);
    
    // Executing the kernel
    fluidCL->runKernel(densityKernel, NullRange, global, local);
}

void Fluid::runLambdaEstimationKernel(){
    // Work groups initialization
    NDRange global(_N);
    NDRange local(1);
    
    // Setting the arguments
    lambdaKernel->setArg(0, *ptNbrBuff);
    lambdaKernel->setArg(1, *partBuff);

    // Executing the kernel
    fluidCL->runKernel(lambdaKernel, NullRange, global, local);
}

void Fluid::runDisplacementKernel(){
    // Work groups initialization
    NDRange global(_N);
    NDRange local(1);

    // Setting the arguments
    displacementKernel->setArg(0, *ptNbrBuff);
    displacementKernel->setArg(1, *partBuff);
    displacementKernel->setArg(2, *particlePosBuff);
    displacementKernel->setArg(3, *voxelInfoBuff);
    displacementKernel->setArg(4, *timeBuff);

    // Executing the kernel
    fluidCL->runKernel(displacementKernel, NullRange, global, local);
}

void Fluid::runCollisionKernel(){
    // Work groups initialization
    NDRange global(_N);
    NDRange local(1);
    
    // Setting the arguments
    collisionKernel->setArg(0, *partBuff);
    collisionKernel->setArg(1, *voxelInfo);
    
    // Executing the kernel
    fluidCL->runKernel(collisionKernel, NullRange, global, local);
}

void Fluid::runVelocityKernel(){
    // Work groups initialization
    NDRange global(_N);
    NDRange local(1);
    
    // Setting the arguments
    velocityKernel->setArg(0, *partBuff);
    velocityKernel->setArg(1, *particlePosBuff);

    // Executing the kernel
    fluidCL->runKernel(velocityKernel, NullRange, global, local);
}
