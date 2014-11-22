#include "structs/structures.h"
//#define poly6(x) 1.5667 * pow(1-x*x, 3)

__kernel void density_estimation(__global struct ParticleNbrs* partNbrs, __global struct Particle* P){
    //Get the index of the current element to be processed
    int i = get_global_id(0);
    
    float density = 0.0;
    float distSquared;
    float diffX, diffY, diffZ;
    //Evaluating the kernel for each voxel
    for(int j = 0; j < 8; j++){
        int nbr = partNbrs[i].nbrParticle[j].voxelId;
        int nbrStartIndex = partNbrs[i].nbrParticle[j].voxelIndex;
        if(nbrStartIndex == -1) continue;
        for(int k = nbrStartIndex ;; k++){
            if(P[k].voxelId != nbr) break;
            diffX = (P[i].pos.x - P[k].pos.x);
            diffY = (P[i].pos.y - P[k].pos.y);
            diffX = (P[i].pos.z - P[k].pos.z);
            distSquared = diffX * diffX + diffY * diffY + diffZ * diffZ;
            if(distSquared <= 1){
                density += 1.5667 * pown(1 - distSquared, 3);
            }
        }
    }
    P[i].density = density;
    return;
}

