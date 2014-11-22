#ifndef PARAMETERS
#define PARAMETERS

#define noParticles (1<<15)
#define evolveTime 0.05
//External force
#define gX 0.0
#define gY -1.0
#define gZ 0.0
//Voxel dimensions
#define voxelSize 2.0
#define noVoxelX 50
#define noVoxelY 50
#define noVoxelZ 50
#define noVoxels noVoxelX * noVoxelY * noVoxelZ
//Particle properties
#define particleSize 0.25
#define inflSize 1.0
#define maxNbrhood 1024

//Parameters for radix sorting
#define noThreads 32
#define maxDigitPower 5

//Parameters for hashing voxels
#define noHashThreads 2

//Parameters for rest density
#define fluidDensity 21.4
#define regularization 0.1
#define prCorrK 0.1 //Pressure correction ratio
#define prPartDistance 0.2//Distance of a near particle for pressure correction
#define prCorrection 0.2441

//relative contribution to pbd
//#define pbdWeight 1.0

//Kernels and derivs
#define poly6(x) 1.5667 * pow(1-x*x, 3)
#define poly6Deriv(x) -9.4000 * x * pow(1-x*x, 2)

#define spiky(x) 4.7746 * pow(1-x, 3)
#define spikyDeriv(x) -14.3238 * pow(1-x, 2)

//OpenGL parameters
#define winHeight 768
#define winWidth 1024
#define heightOffSet 100
#define widthOffSet 100

#define pointSpriteSize 1.0

#define lightPosX 0.69
#define lightPosY -0.69
#define lightPosZ -0.69

//Debugging OpenGL
#define noDebugPts 10
#endif
