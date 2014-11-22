#include "structs/structures.h"

__kernel void hash_histogram(__global struct Particle* P, __global struct VoxelHistogram* voxelHist, __global int* hashParams){
    //hashParams[0] = noHashThreads
    //hashParams[1] = _N/noHashThreads
    //Get the index of the current element to be processed
    int index = get_global_id(0);
    int startIndex = hashParams[1] * index;

    //Initialization for first element
    voxelHist[index].voxels[0].voxelId = P[startIndex].voxelId;
    voxelHist[index].voxels[0].voxelIndex = startIndex;

    int hashIndex = 1;
    for(int i = startIndex + 1; i < startIndex + hashParams[1]; i++){
        if(voxelHist[index].voxels[hashIndex-1].voxelId != P[i].voxelId){
            voxelHist[index].voxels[hashIndex].voxelId = P[i].voxelId;
            voxelHist[index].voxels[hashIndex].voxelIndex = i;
            hashIndex++;
       }
    }
    voxelHist[index].validEntries = P[startIndex + hashParams[1] -1].voxelId;
}
