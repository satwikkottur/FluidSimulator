#include "structs/structures.h"

__kernel void radix_partition(__global struct Histogram* hist, __global struct Histogram* startPt, __global int* digitParams){
    //Get the index of the current element to be processed
    int count = 0;
    for(int j = 0; j < 10; j++){
        for(int i = 0; i < digitParams[3]; i++){
            startPt[i].bins[j] = count;
            count += hist[i].bins[j];
        }         
    }
    return;
}
