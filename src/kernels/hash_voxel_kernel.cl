#include "structs/structures.h"

__kernel void hash_voxel(__global struct VoxelHistogram* voxelHist, __global int* voxelIndex, __global int* hashParams){
    int voxelId = 0;
    int histIndex = -1, prevHistIndex;

    //Hash Param 0 = noHashThreads
    //Hash Param 1 = N / noHashThreads
    for(int i = 0; i < hashParams[0]; i++){
        int j = 0;
        while(histIndex != voxelHist[i].validEntries){
        //for(int j = 0; j < voxelHist[i].validEntries; j++){
            prevHistIndex = histIndex;
            histIndex = voxelHist[i].voxels[j].voxelId;

            while(voxelId < histIndex){
                voxelIndex[voxelId] = -1;
                voxelId++;
            }

            if(histIndex != prevHistIndex){
                voxelIndex[voxelId] = voxelHist[i].voxels[j].voxelIndex; 
                voxelId++;
            }

            /*if(hashIndex == 0){
                voxelHist[index].voxels[hashIndex].voxelId = P[i].voxelId;
                voxelHist[index].voxels[hashIndex].voxelIndex = i;
                hashIndex++;
                continue;
            }

            if(voxelHist[index].voxels[hashIndex-1].voxelId != P[i].voxelId){
                voxelHist[index].voxels[hashIndex].voxelId = P[i].voxelId;
                voxelHist[index].voxels[hashIndex].voxelIndex = i;
                hashIndex++;
                continue;
            }*/
            j++;
        }
    }
}
