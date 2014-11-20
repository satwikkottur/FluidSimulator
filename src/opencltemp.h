#ifndef OPENCL_TEMPLATE
#define OPENCL_TEMPLATE

//#include "opencl.h"

//Buffer writing
template <typename T>
Buffer* OpenCL::writeToBuffer(T* memBlock, int memType, int length){
    Buffer* memBuff = new Buffer(*context, memType, sizeof(T) * length);
    queue->enqueueWriteBuffer(*memBuff, CL_TRUE, 0, length * sizeof(T), memBlock);

    return memBuff;
}

template<typename T>
T* OpenCL::readFromBuffer(Buffer* memBuff, int length){
    T* memory = new T[length];
    queue->enqueueReadBuffer(*memBuff, CL_TRUE, 0, length * sizeof(T), memory);
    
    return memory;
}

template<typename T>
Buffer* OpenCL::createBuffer(int memType, int length){
    Buffer* memBuff = new Buffer(*context, memType, length * sizeof(T));
    
    return memBuff;
}

/*template <typename T>
Buffer* writeToBuffer(OpenCL* cl, T* memBlock, int memType, int length){
    Buffer* memBuff = new Buffer(*(cl->context), memType, sizeof(T) * length);
    cl->queue->enqueueWriteBuffer(*memBuff, CL_TRUE, 0, length * sizeof(T), memBlock);

    return memBuff;
}

emplate<typename T>
T* OpenCL::readFromBuffer(Buffer* memBuff, int length){
    T* memory = new T[length];
    queue->enqueueReadBuffer(*memBuff, CL_TRUE, 0, length * sizeof(T), memory);
    
    return memory;
}

template<typename T>
Buffer* OpenCL::createBuffer(T memTypeName, int memType, int length){
    Buffer* memBuff = new Buffer(*context, memType, length * sizeof(T));
    
    return memBuff;
}*/
#endif
