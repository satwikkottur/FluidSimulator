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

void Fluid::testRadixSort(){
    //Reading back
    particles = fluidCL->readFromBuffer<struct Particle>(partBuff, _N);
    //particles = fluidCL->readFromBuffer<struct Particle>(tempBuff, _N);
    std::cout << "==================================================\n";
    for(int i = 0; i < 32; i++){
        printf("==> %d  (%f %f %f)\n", particles[i].voxelId, particles[i].pos.x, particles[i].pos.y, particles[i].pos.z);
    }
    std::cout << "==================================================\n";
    /*printf("Printing the table for digit %d\n", i);
    std::cout << "=============================================\n";
    particles = fluidCL->readFromBuffer<struct Particle>(partBuff, _N);
    for(int i = 0; i < 32; i++){
        printf("%d \n", particles[i].voxelId);
    }
    std::cout << "=============================================\n";

    hists = fluidCL->readFromBuffer<struct Histogram>(histBuff, noThreads);
    //startPts= fluidCL->readFromBuffer<struct Histogram>(startPtBuff, noThreads);
    for(int i = 0; i < noThreads; i++){
        for(int j = 0; j < 10; j++){
            std::cout << hists[i].bins[j] << " ";
        }
        std::cout << std::endl;
    }
    //std::cout << "=============================================\n";*/
    
    /*for(int i = 0; i < noThreads; i++){
        for(int j = 0; j < 10; j++){
            std::cout << startPts[i].bins[j] << " ";
        }
        std::cout << std::endl;
    }*/
}

void Fluid::testHashKernel(){
    //Debugging particle hashing
    partHist = fluidCL->readFromBuffer<struct ParticleHistogram>(partHistBuff, 1);
    printf("======================\n");
    printf("%d\n", partHist[0].validEntries);

    for(int i = 0; i < 40; i++){
    //for(int i = 0; i < partHist[0].validEntries; i++){
        printf("%d %d %d\n", i, partHist[0].voxels[i].voxelId, partHist[0].voxels[i].voxelIndex);
    }
    printf("======================\n");
}

void Fluid::testDensityEstimationKernel(){
    printf("============================\n");
    printf("Rest density %f\n", findRestDensity());
    printf("============================\n");
}

void Fluid::testNeighborKernel(){
    //Debugging the neighbour finding kernel
    ptNbrs = fluidCL->readFromBuffer<struct ParticleNbrs>(ptNbrBuff, _N);
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 8; j++){
            printf("%d %d | ", ptNbrs[i].nbrParticle[j].voxelId, ptNbrs[i].nbrParticle[j].voxelIndex);
        }
        printf("\n");
    }
}

void Fluid::testLambdaEstimationKernel(){
    //Debugging the lambda calculations
    particles = fluidCL->readFromBuffer<struct Particle>(partBuff, 1);
    for(int i = 0; i < 100; i++){
        printf("%f %f\n", particles[i].lambda, particles[i].density);
    }
}
