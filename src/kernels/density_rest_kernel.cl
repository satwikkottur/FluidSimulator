#include "structs/structures.h"
#include "parameters.h"

__kernel void density_rest(__global struct Particle* P, __global float* restDensity){
    float density = 0.0;
    
    for(int i = 0; i < N; i++){
        density += P[i].density;        
    }
    restDensity[0] = density / N;
}
