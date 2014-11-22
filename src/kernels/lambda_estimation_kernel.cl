#include "structs/structures.h"
#include "parameters.h"

__kernel void lambda_estimation(__global struct ParticleNbrs* partNbrs, __global struct Particle* P){
    //Get the index of the current element to be processed
    int i = get_global_id(0);

    float density = 0.0;
    float distSquared;
    float diffX, diffY, diffZ;
    float denominator = 0;
    float denomPart = 0;
    float nbrContr = 0;
    float dist;
    //Evaluating the kernel for each voxel
    for(int j = 0; j < 8; j++){
        int nbr = partNbrs[i].nbrParticle[j].voxelId;
        int nbrStartIndex = partNbrs[i].nbrParticle[j].voxelIndex;
        if(nbrStartIndex == -1) continue;
        for(int k = nbrStartIndex; ; k++){
            if(P[k].voxelId != nbr) break;
            diffX = (P[i].pos.x - P[k].pos.x);
            diffY = (P[i].pos.y - P[k].pos.y);
            diffX = (P[i].pos.z - P[k].pos.z);
            distSquared = diffX * diffX + diffY * diffY + diffZ * diffZ;
            dist = sqrt(distSquared);
            //Current neighbour is under the support of the kernel function
            if(distSquared <= 1){
                //Density estimation
                density += 1.5667 * pown(1 - distSquared, 3);
                //Contribution of the neighbour
                nbrContr = 14.3238 * pown(1 - dist, 2); 

                denominator += pown(nbrContr, 2);
                denomPart += nbrContr;
            }
        }
    }

    P[i].lambda = -1 * (density/fluidDensity -1) / (regularization + (denominator + pown(denomPart, 2))/pown(fluidDensity, 2)); 
    P[i].density = density;
    return;
}

