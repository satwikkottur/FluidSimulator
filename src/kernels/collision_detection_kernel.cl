#include "structs/structures.h"

__kernel void collision_detection(__global struct Particle* P, __global int* voxelInfo){
    //Get the index of the current element to be processed
    int i = get_global_id(0);
    int voxelSize = voxelInfo[0];
    int noVoxelX = voxelInfo[1];
    int noVoxelY = voxelInfo[2];
    int noVoxelZ = voxelInfo[3];


    /*if(P[i].pos.x < 0) P[i].pos.x *= -1;    
    if(P[i].pos.y < 0) P[i].pos.y *= -1;    
    if(P[i].pos.z < 0) P[i].pos.z *= -1;    
    if(P[i].pos.x > voxelSize * noVoxelX) P[i].pos.x = 2 * voxelSize * noVoxelX - P[i].pos.x;    
    if(P[i].pos.y > voxelSize * noVoxelY) P[i].pos.y = 2 * voxelSize * noVoxelY - P[i].pos.y;    
    if(P[i].pos.z > voxelSize * noVoxelZ) P[i].pos.z = 2 * voxelSize * noVoxelZ - P[i].pos.z;    */
    
    P[i].pos.x = 0.0;
    P[i].pos.y = 0.0;
    P[i].pos.z = 0.0;

    return;
}

