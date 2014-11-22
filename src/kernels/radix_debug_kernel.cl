//#include "structs/structures.h"

__kernel void radix_debug(__global struct Particle* P, __global struct Particle* Q, __global struct Histogram* hist, __global int* digitParams){
    //Get the index of the current element to be processed
    int index = get_global_id(0);
    struct Histogram curCount;
    for(int i = 0; i < 10; i++){
        curCount.bins[i] = 0;        
    }
    int startIndex = digitParams[2]*index;
    int extDigit, shiftIndex;
    for(int i = startIndex; i < startIndex + digitParams[2]; i++){
        extDigit = (P[i].voxelId/digitParams[0]) % 10;
        shiftIndex = hist[index].bins[extDigit] + curCount.bins[extDigit];
        curCount.bins[extDigit]++;
        //Q[shiftIndex] = P[i];
        //Q[i] = P[i];
        Q[i].voxelId = hist[index].bins[extDigit];
    }
    return;
}
