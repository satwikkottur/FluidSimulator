#include "structs/structures.h"

__kernel void hash_particle(__global struct ParticleHistogram* partHist, __global struct Particle* P, __global int* voxelInfo){
    int noValid = 0;
    int noParticles = voxelInfo[5];
    
    //Initializing the first element of the histogram with the first particle
    partHist[0].voxels[noValid].voxelId = P[0].voxelId;
    partHist[0].voxels[noValid].voxelIndex = 0;
    noValid++;

    for(int i = 1; i < noParticles; i++){
        //Different voxel encountered that needs to be registered
        if(P[i].voxelId != P[i-1].voxelId){
            partHist[0].voxels[noValid].voxelId = (int)P[i].voxelId;
            partHist[0].voxels[noValid].voxelIndex = i;
            noValid++;
        }
    } 
    partHist[0].validEntries = noValid;
    return;
}
