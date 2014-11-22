#include "structs/structures.h"
#include "parameters.h"

__kernel void copy_position(__global struct Particle* P, __global float* particlePos){
    //Get the index of the current element to be processed
    int i = get_global_id(0);
    particlePos[3 * i] = P[i].pos.x;
    particlePos[3 * i + 1] = P[i].pos.y;
    particlePos[3 * i + 2] = P[i].pos.z;
    return;
}

