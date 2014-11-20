#ifndef OPENCL_FUNCS
#define OPENCL_FUNCS

#define __NO_STD_VECTOR
#define _CL_ENABLE_EXCEPTIONS
#ifdef __APPLE__
    #include <OpenCL/cl.hpp>
    //#include "GL/glx.h"
    #include <OpenCL/cl_gl.h> 
    //#include <OpenGL/OpenGL.h> 
    //#include "cl_gl_ext.h"
    //include <GLUT/freeglut.h> 
    //#define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing" 
#elif
    #include "CL/cl.hpp"
    #include "GL/glx.h"
#endif
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <map>

using namespace cl;

//Class to take care of all the hassles and programming syntaxes from opencl
class OpenCL{
    public:
        //Kernel object map
        std::map<std::string, Kernel*> kernels;
        //Kernel* kernel;

        ///////////////////////////////////////////////////////////////////////////
        //Methods
        //Blank constructor
        OpenCL();
        //Initializing the opencl setup
        void initOpenCL();
        //Initializing the opencl setup with opengl context
        void initOpenCLwithOpenGL();
        //Setting up the kernels
        Kernel* createKernel(std::string kernelFile, std::string kernelName);
        //Adding the kernel
        void addKernel(Kernel* kernel, std::string kernelName);
        //Running the kernel
        void runKernel(Kernel* kernel, NDRange offset, NDRange globalRange, NDRange localRange);
        //Flushing the queue
        void flushQueue();

        //Functions included in another template header
        //Function to create/write to a buffer
        template <typename T>
        Buffer* writeToBuffer(T* memBlock, int memType, int length);
        //Function to read from buffer
        template <typename T>
        T* readFromBuffer(Buffer* memBuff, int length);
        //Function to create empty memory
        template <typename T>
        Buffer* createBuffer(int memType, int length);

        ///////////////////////////////////////////////////////////////////////////
    public:
        //Attributes
        //Available platforms
        vector<Platform> platforms;
        //OpenCL context
        Context* context;
        //Available devices
        vector<Device> devices;
        //Command Queue for operations
        CommandQueue* queue;
        
        ///////////////////////////////////////////////////////////////////////////
        //Methods
};

#include "opencltemp.h"
#endif
