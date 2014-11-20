#ifndef STRUCTURES
#define STRUCTURES
//#include "../parameters.h"

#define dist(a, b) sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z))
#define distSq(a, b) (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z)

struct Tuple{
    float x;
    float y;
    float z;
};

//Structure for a particle
struct Particle{
    //Position of the particle
    struct Tuple pos;
    //Velocity of the particle
    struct Tuple vel;
    //Acceleration of the particle
    struct Tuple acc;
    //Voxel id of the particle
    int voxelId;
    //Density of the particle
    float density;
    //Lambda value of the particle
    float lambda;
};

struct Histogram{
    //Histogram bins (rigid for now)
    int bins[10];
};

struct Voxel{
    int voxelId;
    int voxelIndex;
};

/*struct VoxelHistogram{
    struct Voxel voxels[(noParticles)/noHashThreads];
    int validEntries;
};*/

struct ParticleHistogram{
    struct Voxel voxels[(1<<15)];
    int validEntries;
};

struct ParticleNbrs{
    struct Voxel nbrParticle[8];
};
#endif
