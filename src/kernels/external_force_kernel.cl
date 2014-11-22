#include "structs/structures.h"

__kernel void external_force(__global struct Particle* P, __global struct Tuple* accel, __global float* timeStep, __global int* voxelInfo){
 
    //Get the index of the current element to be processed
    int i = get_global_id(0);
    float accX = accel[0].x;
    float accY = accel[0].y;
    float accZ = accel[0].z;
    float time = timeStep[0];
 
    //Updating the velocity
    P[i].vel.x += accX * time;
    P[i].vel.y += accY * time;
    P[i].vel.z += accZ * time;

    //Updating displacement
    P[i].pos.x += P[i].vel.x * time;
    P[i].pos.y += P[i].vel.y * time;
    P[i].pos.z += P[i].vel.z * time;

    int voxelSize = voxelInfo[0];
    int noVoxelX = voxelInfo[1];
    int noVoxelY = voxelInfo[2];
    int noVoxelZ = voxelInfo[3];

    //Updating voxel id based on the current position
    int voxelX = P[i].pos.x/voxelSize;
    int voxelY = P[i].pos.y/voxelSize;
    int voxelZ = P[i].pos.z/voxelSize;

    //Checking for boundary cases and clamping them
    voxelX = (voxelX < 0)? 0 : voxelX;
    voxelX = (voxelX > noVoxelX -1)? noVoxelX -1 : voxelX;
    voxelY = (voxelY < 0)? 0 : voxelY;
    voxelY = (voxelY > noVoxelY -1)? noVoxelY -1 : voxelY;
    voxelZ = (voxelZ < 0)? 0 : voxelZ;
    voxelZ = (voxelZ > noVoxelZ -1)? noVoxelZ -1 : voxelZ;

    P[i].voxelId = voxelX + voxelY * noVoxelX + voxelZ * noVoxelY;
    return;
}
