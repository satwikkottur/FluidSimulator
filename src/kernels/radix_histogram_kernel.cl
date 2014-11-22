#include "structs/structures.h"

__kernel void radix_histogram(__global struct Particle* P, __global struct Histogram* hist, __global int* digitParams){
    //Get the index of the current element to be processed
    int index = get_global_id(0);
    int startIndex = digitParams[2]*index;
    int extDigit;
    for(int i = startIndex; i < startIndex + digitParams[2]; i++){
        extDigit = (P[i].voxelId/digitParams[0]) % 10;
        hist[index].bins[extDigit] += 1;
    }
    return;
}
