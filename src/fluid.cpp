#include "fluid.h"

Fluid::Fluid(){
    _N = noParticles;
    _t = evolveTime;

    //Initializing the particles
    particles = new struct Particle[_N];
    for(int i = 0 ; i < _N ; i++){
        particles[i].pos.x = particles[i].pos.y = particles[i].pos.z = 0;
        particles[i].vel.x = particles[i].vel.y = particles[i].vel.z = 0;
        particles[i].acc.x = particles[i].acc.y = particles[i].acc.z = 0;
        particles[i].density = 2.0;
        particles[i].voxelId = (i % 7);
    }

    particlePos = new float[3 * _N];
}

void Fluid::initParticles(std::string initState){
    float radius = 2*particleSize;
    float height = 10;
    if(initState.compare("BLOCK_IN_AIR") == 0){
        int dim = pow(_N, 1.0/3.0);
        int dimSq = dim * dim;
        for(int i = 0 ; i < _N; i++){
            //Arranging them in a cube
            //particles[i].pos = {(i%dim) * radius, ((i/dim)%dim * radius) + height, (i/dimSq) * radius};
            particles[i].pos.x = (i%dim) * radius;
            particles[i].pos.y = ((i/dim)%dim * radius) + height;
            particles[i].pos.z = (i/dimSq) * radius;
            particles[i].density = 21.0;
            particles[i].voxelId = 10 + i%7;
        } 
        /*for(int i = 0; i < _N-1; i++){
            printf("%f %f %f => %f\n", particles[i].pos.x, particles[i].pos.y, particles[i].pos.z, distSq(particles[i].pos, particles[i+1].pos));
        }*/
        std::cout << "Particle system initialized in " << initState << " configuration\n";
    }    
    transferDataToBuffer();
}

void Fluid::setupKernels(){
    //Initializing the setup for simulations
        
    std::string strings[] = {"external_force" , "radix_histogram", "radix_partition",
                            "radix_copy", "radix_resetParams", "hash_particle",
                            "density_estimation", "lambda_estimation", "find_neighbour",
                            "displacement_estimation", "collision_detection", "copy_position", 
                            "velocity_update"};
    //Creating kernels
    std::string kernelFile, kernelName;

    //Initializing kernels from the above list 
    printf("\n\n=====================================================\nBuilding kernels");
    printf("\n======================================================\n");
    for(int i = 0; i < sizeof(strings)/sizeof(strings[0]); i++){
        kernelFile = "kernels/" + strings[i] + "_kernel.cl";
        kernelName = strings[i];
        fluidCL->createKernel(kernelFile, kernelName);
        printf("Kernel %s setup at location : %s\n", kernelName.c_str(), kernelFile.c_str()) ;
    }
    printf("\n=====================================================\n\n");
}

void Fluid::setupSystem(){
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Setting up things for radix Sort
    histKernel = fluidCL->kernels.at("radix_histogram");
    hists = new Histogram[noThreads];
    for(int i = 0; i < noThreads; i++){
        for(int j = 0; j < 10; j++){
            hists[i].bins[j] = 0;
        }
    }

    histBuff = fluidCL->writeToBuffer<struct Histogram>(hists, CL_MEM_READ_WRITE, noThreads);

    digitPower = new int[4];
    digitPower[0] = 1;
    digitPower[1] = _N;
    digitPower[2] = _N/noThreads;
    digitPower[3] = noThreads;
    digitBuff = fluidCL->writeToBuffer<int>(digitPower, CL_MEM_READ_WRITE, 4);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    partKernel = fluidCL->kernels.at("radix_partition");
    startPts = new Histogram[noThreads];
    startPtBuff = fluidCL->createBuffer<struct Histogram>(CL_MEM_READ_WRITE, noThreads);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    copyKernel = fluidCL->kernels.at("radix_copy");
    sortParticles = new Particle[_N];
    sortPartBuff = fluidCL->createBuffer<struct Particle>(CL_MEM_READ_WRITE, _N);

    //Debug kernel
    //Kernel* debugKernel = fluidCL->kernels.at("radix_debug");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    resetKernel = fluidCL->kernels.at("radix_resetParams");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///Setting up the kernel for execution
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    updateKernel = fluidCL->kernels.at("external_force");
    
    acceleration = new Tuple[1];
    acceleration[0].x = gX;
    acceleration[0].y = gY;
    acceleration[0].z = gZ;
    accBuff = fluidCL->writeToBuffer<struct Tuple>(acceleration, CL_MEM_READ_ONLY, 1);

    timeStep = new float[1];
    timeStep[0] = _t;
    timeBuff = fluidCL->writeToBuffer<float>(timeStep, CL_MEM_READ_ONLY, 1);
    //Buffer to hold the information about the voxels
    voxelInfo = new int[6];
    voxelInfo[0] = voxelSize;
    voxelInfo[1] = noVoxelX;
    voxelInfo[2] = noVoxelY;
    voxelInfo[3] = noVoxelZ;
    voxelInfo[4] = noVoxels;
    voxelInfo[5] = _N;
    voxelInfoBuff = fluidCL->writeToBuffer<int>(voxelInfo, CL_MEM_READ_ONLY, 6);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*hashKernel = fluidCL->kernels.at("hash_histogram");

    hashParams = new int[2];
    hashParams[0] = noHashThreads;
    hashParams[1] = _N/noHashThreads;
    hashParamBuff = fluidCL->writeToBuffer<int>(hashParams, CL_MEM_READ_ONLY, 2);

    voxelHist = new VoxelHistogram[noHashThreads];
    voxelHistBuff = fluidCL->createBuffer<VoxelHistogram>(CL_MEM_READ_WRITE, noHashThreads);
    voxelIndexBuff = fluidCL->createBuffer<int>(CL_MEM_READ_WRITE, noVoxelX * noVoxelY * noVoxelZ);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    voxelKernel = fluidCL->kernels.at("hash_voxel");*/
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    partHashKernel = fluidCL->kernels.at("hash_particle");
    partHist = new ParticleHistogram[1];
    partHistBuff = fluidCL->createBuffer<struct ParticleHistogram>(CL_MEM_READ_WRITE, 1);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    densityKernel = fluidCL->kernels.at("density_estimation");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Average rest density estimation
    lambdaKernel = fluidCL->kernels.at("lambda_estimation");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Neighbour finding kernel
    nbrKernel = fluidCL->kernels.at("find_neighbour");
    ptNbrBuff = fluidCL->createBuffer<struct ParticleNbrs>(CL_MEM_READ_WRITE, _N);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    displacementKernel = fluidCL->kernels.at("displacement_estimation");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Collision handling
    collisionKernel = fluidCL->kernels.at("collision_detection");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Kernel to copy vertex information to float array
    posCopyKernel = fluidCL->kernels.at("copy_position");
    particlePosBuff = fluidCL->createBuffer<float>(CL_MEM_READ_WRITE, 3 * _N);
    sortParticlePosBuff = fluidCL->createBuffer<float>(CL_MEM_READ_WRITE, 3 * _N);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Updating the velocity
    velocityKernel = fluidCL->kernels.at("velocity_update");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Fluid::transferDataToBuffer(){
    partBuff = fluidCL->writeToBuffer<struct Particle>(particles, CL_MEM_READ_WRITE, _N);
}

void Fluid::runIteration(){
    //Worksize definitions
    NDRange global(_N);
    NDRange local(1);
    //Variables for external factors calculation
    struct Tuple extAcc = {gX, gY, gZ};
    Buffer* extAccBuff = fluidCL->writeToBuffer<struct Tuple>(&extAcc, CL_MEM_READ_ONLY, 1);
    Buffer* timeStepBuff = fluidCL->writeToBuffer<float>(&_t, CL_MEM_READ_ONLY, 1);

    //Variables for histogram
    int* histogram = new int[10];
    for(int i = 0 ; i < 10; i++){
        histogram[i] = 0;
    }
    histBuff = fluidCL->writeToBuffer<int>(histogram, CL_MEM_READ_WRITE, 10);
    int digitPower = 1;
    digitBuff = fluidCL->writeToBuffer<int>(&digitPower, CL_MEM_READ_WRITE, 1);

    //First step of updating based on external factors
    Kernel* updateKernel = fluidCL->kernels.at("external_force");
    updateKernel->setArg(0, partBuff);
    updateKernel->setArg(1, extAccBuff);
    updateKernel->setArg(2, timeStepBuff);

    for(int i = 0; i < 1; i++){
        fluidCL->runKernel(updateKernel, NullRange, global, local);
    }

    fluidCL->flushQueue();
    particles = fluidCL->readFromBuffer<struct Particle>(partBuff, _N);

    for(int i = 0; i < 10; i++){
        //std::cout << i << " " << histogram[i] << std::endl;
    }

    for(int i = 0; i < 100; i++){
        printf("%f %f %f %d\n", particles[i].pos.x, particles[i].pos.y, particles[i].pos.z, particles[i].voxelId);
    }
}

void Fluid::debug(BufferGL* vertexBuffOpenGL){
    //Work groups initialization
    NDRange global(_N);
    NDRange local(1);
    NDRange hash(noHashThreads);

    //particles = fluidCL->readFromBuffer<struct Particle>(partBuff, 1);
    //printParticleInfo(0);
    particlePos = fluidCL->readFromBuffer<float>(particlePosBuff, 3 * _N);
    //printf("%f %f %f\n", particlePos[0], particlePos[1], particlePos[2]);
    //printf("%f %f %f\n", (particles[0].pos.x-particlePos[0])/evolveTime, (particles[0].pos.y-particlePos[1])/evolveTime, (particles[0].pos.z-particlePos[2])/evolveTime);
    printf("===========================================\n");
        posCopyKernel->setArg(0, *partBuff);
        posCopyKernel->setArg(1, *particlePosBuff);
        fluidCL->runKernel(posCopyKernel, NullRange, global, local);

        updateKernel->setArg(0, *partBuff);
        updateKernel->setArg(1, *accBuff);
        updateKernel->setArg(2, *timeBuff);
        updateKernel->setArg(3, *voxelInfoBuff);
        fluidCL->runKernel(updateKernel, NullRange, global, local);

        //Sorting the voxels
        radixGPU();

        //Hashing the particles
        partHashKernel->setArg(0, *partHistBuff);
        partHashKernel->setArg(1, *partBuff);
        partHashKernel->setArg(2, *voxelInfoBuff);
        fluidCL->runKernel(partHashKernel, NullRange, local, local);

        //Finding the neighbours
        nbrKernel->setArg(0, *ptNbrBuff);
        nbrKernel->setArg(1, *partBuff);
        nbrKernel->setArg(2, *partHistBuff);
        nbrKernel->setArg(3, *voxelInfoBuff);
        fluidCL->runKernel(nbrKernel, NullRange, global, local);

        //To find the rest density for the first time
        /*if(i == 0){
            densityKernel->setArg(0, *ptNbrBuff);
            densityKernel->setArg(1, *partBuff);
            fluidCL->runKernel(densityKernel, NullRange, global, local);
        }*/

   //Running the updating sep
    for(int i = 0 ; i < 1; i++){
        lambdaKernel->setArg(0, *ptNbrBuff);
        lambdaKernel->setArg(1, *partBuff);
        fluidCL->runKernel(lambdaKernel, NullRange, global, local);

        displacementKernel->setArg(0, *ptNbrBuff);
        displacementKernel->setArg(1, *partBuff);
        displacementKernel->setArg(2, *particlePosBuff);
        displacementKernel->setArg(3, *voxelInfoBuff);
        displacementKernel->setArg(4, *timeBuff);
        fluidCL->runKernel(displacementKernel, NullRange, global, local);

        /*collisionKernel->setArg(0, *partBuff);
        collisionKernel->setArg(1, *voxelInfo);
        fluidCL->runKernel(collisionKernel, NullRange, global, local);*/

    }
        velocityKernel->setArg(0, *partBuff);
        velocityKernel->setArg(1, *particlePosBuff);
        fluidCL->runKernel(velocityKernel, NullRange, global, local);

        /*posCopyKernel->setArg(0, *partBuff);
        posCopyKernel->setArg(1, *vertexBuffOpenGL);
        fluidCL->runKernel(posCopyKernel, NullRange, global, local);*/

        fluidCL->flushQueue();
    
    //Debugging the neighbour finding kernel
    /*ptNbrs = fluidCL->readFromBuffer<struct ParticleNbrs>(ptNbrBuff, _N);
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 8; j++){
            printf("%d %d | ", ptNbrs[i].nbrParticle[j].voxelId, ptNbrs[i].nbrParticle[j].voxelIndex);
        }
        printf("\n");
    }*/

    //Debugging particle hashing
    /*partHist = fluidCL->readFromBuffer<struct ParticleHistogram>(partHistBuff, 1);
    printf("======================\n");
    printf("%d\n", partHist[0].validEntries);
    for(int i = 0; i < 40; i++){
    //for(int i = 0; i < partHist[0].validEntries; i++){
        printf("%d %d %d\n", i, partHist[0].voxels[i].voxelId, partHist[0].voxels[i].voxelIndex);
    }
    printf("======================\n");*/

    //Debugging
    //VoxelHistogram* readHist = fluidCL->readFromBuffer<VoxelHistogram>(voxelHistBuff, noHashThreads);
    
    /*particles = fluidCL->readFromBuffer<struct Particle>(partBuff, _N);
    for(int i = 0; i < 10; i++){
        printf("%d \n", particles[i].voxelId);
    }*/
    //Debugging
    /*std::cout << "==============\n";
    for(int i = 0; i < noHashThreads; i++){
        std::cout << readHist[i].validEntries << std::endl;
    }
    std::cout << "==============\n";
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < noHashThreads; j++){
            std::cout << readHist[j].voxels[i].voxelId << " "<< readHist[j].voxels[i].voxelIndex<< "  ";
        }
        std::cout << std::endl;
    }
    std::cout << "==============\n";*/

    /*int* index = fluidCL->readFromBuffer<int>(voxelIndexBuff, noVoxelX * noVoxelY * noVoxelZ);
    //for(int i = 0; i < 5500; i++){
    for(int i = 15200; i < 15250; i++){
        if(index[i] != -1){
            std::cout << index[i] << " " << i << std::endl;
        }
    }
    std::cout << "==============\n";*/

    /*printf("============================\n");
    printf("Rest density %f\n", findRestDensity());
    printf("============================\n");*/


    //Debugging the lambda calculations
    //particles = fluidCL->readFromBuffer<struct Particle>(partBuff, 1);
    //printf("%f %f (%f %f %f) (%f %f %f)\n", particles[0].lambda, particles[0].density, particles[0].pos.x, particles[0].pos.y, particles[0].pos.z,
                                            //particles[0].vel.x, particles[0].vel.y, particles[0].vel.z);
    /*for(int i = 0; i < 100; i++){
        printf("%f %f\n", particles[i].lambda, particles[i].density);
    }*/
}

void Fluid::radixCPU(){
    int powerDigit = 10000;
    int histogram[10];
    for(int i = 0 ; i < 10; i++){
        histogram[i] = 0;
    }
    for(int i = 0; i < _N; i++){
        histogram[(particles[i].voxelId/powerDigit) %1]++;  
    }
}

void Fluid::radixGPU(){
    NDRange threads(noThreads);
    NDRange global(_N);
    NDRange local(1);
    
    digitPower = new int[4];
    digitPower[0] = 1;
    digitPower[1] = _N;
    digitPower[2] = _N/noThreads;
    digitPower[3] = noThreads;
    digitBuff = fluidCL->writeToBuffer<int>(digitPower, CL_MEM_READ_WRITE, 4);
 
    //Running the kernel
    int noIter = maxDigitPower;
    Buffer* tempPointer;
    for(int i = 0; i < noIter; i++){
        //Setting the arguements
        histKernel->setArg(0, *partBuff);
        histKernel->setArg(1, *histBuff);
        histKernel->setArg(2, *digitBuff);
        fluidCL->runKernel(histKernel, NullRange, threads, local);

        partKernel->setArg(0, *histBuff);
        partKernel->setArg(1, *startPtBuff);
        partKernel->setArg(2, *digitBuff);
        fluidCL->runKernel(partKernel, NullRange, local, local);

        copyKernel->setArg(0, *partBuff);
        copyKernel->setArg(1, *sortPartBuff);
        copyKernel->setArg(2, *particlePosBuff);
        copyKernel->setArg(3, *sortParticlePosBuff);
        copyKernel->setArg(4, *startPtBuff);
        copyKernel->setArg(5, *digitBuff);
        fluidCL->runKernel(copyKernel, NullRange, threads, local);

        //Debugging
        /*debugKernel->setArg(0, *partBuff);
        debugKernel->setArg(1, *sortPartBuff);
        debugKernel->setArg(2, *startPtBuff);
        debugKernel->setArg(3, *digitBuff);*/
        //////////////////////////////////
        //Debugging
        resetKernel->setArg(0, *histBuff);
        resetKernel->setArg(1, *digitBuff);
        fluidCL->runKernel(resetKernel, NullRange, local, local);

        tempBuff = partBuff;
        partBuff = sortPartBuff;
        sortPartBuff = tempBuff;

        tempPointer = sortParticlePosBuff;
        sortParticlePosBuff = particlePosBuff;
        particlePosBuff = tempPointer;
    }

    //Reading back
    /*particles = fluidCL->readFromBuffer<struct Particle>(partBuff, _N);
    //particles = fluidCL->readFromBuffer<struct Particle>(tempBuff, _N);
    std::cout << "==================================================\n";
    for(int i = 0; i < 32; i++){
        printf("==> %d  (%f %f %f)\n", particles[i].voxelId, particles[i].pos.x, particles[i].pos.y, particles[i].pos.z);
    }
    std::cout << "==================================================\n";*/
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

float Fluid::findRestDensity(){
    //Finding the rest density
    particles = fluidCL->readFromBuffer<Particle>(partBuff, _N);
    float density = 0.0;
    for(int i = 0; i < _N; i++){
        density += particles[i].density;
    }
    density /= _N;

    /*for(int i = 0; i < 1000; i++){
        printf("%f ", particles[i].density);
    }*/

    return density;
}

void Fluid::printParticleInfo(int index){
    printf("%f %f %d (%f %f %f) (%f %f %f)\n", particles[index].density, particles[index].lambda, particles[index].voxelId,
                                            particles[index].pos.x, particles[index].pos.y, particles[index].pos.z,
                                            particles[index].vel.x, particles[index].vel.y, particles[index].vel.z);
}

    /*kernelFile = "kernels/external_force_kernel.cl";
    kernelName = "external_force";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/radix_histogram_kernel.cl";
    kernelName = "radix_histogram";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/radix_partition_kernel.cl";
    kernelName = "radix_partition";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/radix_copy_kernel.cl";
    kernelName = "radix_copy";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/radix_resetParams_kernel.cl";
    kernelName = "radix_resetParams";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/hash_particle_kernel.cl";
    kernelName = "hash_particle";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/hash_histogram_kernel.cl";
    kernelName = "hash_histogram";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/hash_voxel_kernel.cl";
    kernelName = "hash_voxel";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/density_estimation_kernel.cl";
    kernelName = "density_estimation";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/lambda_estimation_kernel.cl";
    kernelName = "lambda_estimation";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/find_neighbour_kernel.cl";
    kernelName = "find_neighbour";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/displacement_estimation_kernel.cl";
    kernelName = "displacement_estimation";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/collision_detection_kernel.cl";
    kernelName = "collision_detection";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/copy_position_kernel.cl";
    kernelName = "copy_position";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/velocity_update_kernel.cl";
    kernelName = "velocity_update";
    fluidCL->createKernel(kernelFile, kernelName);

    kernelFile = "kernels/radix_debug_kernel.cl";
    kernelName = "radix_debug";
    fluidCL->createKernel(kernelFile, kernelName);*/