#include "structs/structures.h"

__kernel void displacement_estimation(__global struct ParticleNbrs* partNbrs, __global struct Particle* P, __global float* particlePos, __global int* voxelInfo, __global float* timeStep){
    //Get the index of the current element to be processed
    int i = get_global_id(0);

    float diffX, diffY, diffZ;
    float nbrContrX = 0, nbrContrY = 0, nbrContrZ = 0;
    float dist, distSquared, kernelTerm;
    float scorr = 0;
    //Constants to be optimized later
    float pbdWeight = 1.0;
    float fluidDensity = 21.4;
    float prCorrection = 0.1;

    //Evaluating the kernel for each voxel
    for(int j = 0; j < 8; j++){
        int nbr = partNbrs[i].nbrParticle[j].voxelId;
        int nbrStartIndex = partNbrs[i].nbrParticle[j].voxelIndex;
        if(nbrStartIndex == -1) continue;
        for(int k = nbrStartIndex ; ; k++){
            if(P[k].voxelId != nbr) break;
            diffX = (P[i].pos.x - P[k].pos.x);
            diffY = (P[i].pos.y - P[k].pos.y);
            diffX = (P[i].pos.z - P[k].pos.z);
            distSquared = (diffX * diffX + diffY * diffY + diffZ * diffZ);
            dist = sqrt(distSquared);
            //Current neighbour is under the support of the kernel function
            if(dist <= 1 && dist > 0.0001){
                //Finding the tensible instability term in the expression
                scorr = -1 * prCorrection * pown(1-distSquared, 4);
                
                //Contribution of the neighbour
                kernelTerm = -14.3238 * pown(1-dist, 2)/dist;
                nbrContrX += (P[i].lambda + P[k].lambda + scorr) * diffX * kernelTerm; 
                nbrContrY += (P[i].lambda + P[k].lambda + scorr) * diffY * kernelTerm; 
                nbrContrZ += (P[i].lambda + P[k].lambda + scorr) * diffZ * kernelTerm; 
            }
        }
    } 

    P[i].pos.x += pbdWeight * nbrContrX / fluidDensity;
    P[i].pos.y += pbdWeight * nbrContrY / fluidDensity;
    P[i].pos.z += pbdWeight * nbrContrZ / fluidDensity;

    //Handling collisions
    int voxelSize = voxelInfo[0];
    int noVoxelX = voxelInfo[1];
    int noVoxelY = voxelInfo[2];
    int noVoxelZ = voxelInfo[3];

    if(P[i].pos.x < 0) P[i].pos.x *= -1;    
    if(P[i].pos.y < 0) P[i].pos.y *= -1;    
    if(P[i].pos.z < 0) P[i].pos.z *= -1;    
    if(P[i].pos.x > voxelSize * noVoxelX) P[i].pos.x = 2 * voxelSize * noVoxelX - P[i].pos.x;    
    if(P[i].pos.y > voxelSize * noVoxelY) P[i].pos.y = 2 * voxelSize * noVoxelY - P[i].pos.y;    
    if(P[i].pos.z > voxelSize * noVoxelZ) P[i].pos.z = 2 * voxelSize * noVoxelZ - P[i].pos.z;

    float time = timeStep[0];
    P[i].vel.x = 20 * (P[i].pos.x - particlePos[3 * i]) ;
    P[i].vel.y = 20 * (P[i].pos.y - particlePos[3 * i + 1]) ;
    P[i].vel.z = 20 * (P[i].pos.z - particlePos[3 * i + 2]) ;
    return;  
}

