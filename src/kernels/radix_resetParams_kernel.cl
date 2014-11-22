#include "structs/structures.h"

__kernel void radix_resetParams(__global struct Histogram* hists, __global int* digitParams){
    //Get the index of the current element to be processed
    for(int i = 0; i < digitParams[3];i++){
        for(int j = 0; j < 10; j++){
            hists[i].bins[j] = 0;                
        }
    }
    digitParams[0] *= 10;
    return;
}
