#include "structs/structures.h"

__kernel void find_neighbour(__global struct ParticleNbrs* ptNbrs, __global struct Particle* P, __global struct ParticleHistogram* partHist, __global int* voxelInfo){
    //Get the index of the current element to be processed
    int i = get_global_id(0);
    int voxelSize = voxelInfo[0];
    int noVoxelX = voxelInfo[1];
    int noVoxelY = voxelInfo[2];
    int noVoxels = voxelInfo[4];


    //Finding the neighbours on the basis on position of particle
    float voxelPartX = ((float)P[i].pos.x / voxelSize); 
    float voxelPartY = ((float)P[i].pos.y / voxelSize); 
    float voxelPartZ = ((float)P[i].pos.z / voxelSize); 
    voxelPartX = voxelPartX - (int)voxelPartX;
    voxelPartY = voxelPartY - (int)voxelPartY;
    voxelPartZ = voxelPartZ - (int)voxelPartZ;

    int offsetX = (voxelPartX > 0.5)? 1 : -1;
    int offsetY = (voxelPartY > 0.5)? noVoxelX : -1 * noVoxelX;
    int offsetZ = (voxelPartZ > 0.5)? noVoxelX * noVoxelY : -1 * noVoxelX * noVoxelY;

    for(int j = 0; j < 8; j++){
        ptNbrs[i].nbrParticle[j].voxelId = (int)(P[i].voxelId + (j&1)*offsetX + ((j>>1)&1)*offsetY + ((j>>2)&1)*offsetZ);
    }
    
    //Evaluating the kernel for each voxel
    for(int j = 0; j < 8; j++){
        int nbr = ptNbrs[i].nbrParticle[j].voxelId;
        if(nbr < 0 || nbr > noVoxels) {
            ptNbrs[i].nbrParticle[j].voxelIndex = -1;
            continue;
        }

        //Binary search to find out the neighbour starting index in the particles array
        //Available voxelIds are less than required
        if(partHist[0].voxels[partHist[0].validEntries-1].voxelId < nbr){
            ptNbrs[i].nbrParticle[j].voxelIndex = -1;
            continue;
        }

        //Available voxelIds are more than required
        if(partHist[0].voxels[0].voxelId > nbr){
            ptNbrs[i].nbrParticle[j].voxelIndex = -1;
            continue;
        }

        //Starting the binary search
        int upper, lower, current;
        lower = 0;
        upper = partHist[0].validEntries-1;
        ptNbrs[i].nbrParticle[j].voxelIndex = -1;
        while(lower <= upper){
            current = (upper + lower)/2;

            if(partHist[0].voxels[current].voxelId < nbr){
                lower = current + 1;
            }
            else{
                if(partHist[0].voxels[current].voxelId > nbr){
                    upper = current - 1; 
                }
                else{
                    ptNbrs[i].nbrParticle[j].voxelIndex = partHist[0].voxels[current].voxelIndex;
                    break;
                }
            }
        }
    }
    return;
}

