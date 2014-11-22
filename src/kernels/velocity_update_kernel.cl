#include "structs/structures.h"

__kernel void velocity_update(__global struct Particle* P, __global float* particlePos, __global float* timeStep){
    //Get the index of the current element to be processed
    int i = get_global_id(0);

    float time = timeStep[0];

    P[i].vel.x = 1/time *  (-1 * particlePos[3 * i] + P[i].pos.x);
    P[i].vel.y = 1/time * (-1 * particlePos[3 * i + 1] + P[i].pos.y);
    P[i].vel.z = 1/time * (-1 * particlePos[3 * i + 2] + P[i].pos.z);
    return;
}

