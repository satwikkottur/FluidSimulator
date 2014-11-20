#include "clutils.h"

OpenCL::OpenCL(){
}

//Setting up the initialization
void OpenCL::initOpenCL(){
    if(Platform::get(&platforms) != CL_SUCCESS){
        std::cout << "Searching platforms failed\n";
        exit(0);
    }

    //Printing the name of the platform used 
    std::cout << "Using platform: " << platforms[0].getInfo<CL_PLATFORM_NAME>() << "\nTotal platforms: " << platforms.size() << std::endl;
    
    // Select the default platform and create a context using this platform and the GPU
    cl_context_properties cps[3] = { 
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)(platforms[0])(), 
        0 
    };

    //context = new Context( CL_DEVICE_TYPE_CPU, cps);
    context = new Context( CL_DEVICE_TYPE_GPU, cps);

    // Get a list of devices on this platform
    devices = context->getInfo<CL_CONTEXT_DEVICES>();

    //Printing the name of the default device used
    std::cout<< "Using device: " << devices[1].getInfo<CL_DEVICE_NAME>()<<"\nTotal devices: " << devices.size() << "\n";

    // Create a command queue and use the first device
    queue = new CommandQueue(*context, devices[1]);
}

//Setting up the initialization
void OpenCL::initOpenCLwithOpenGL(){
    if(Platform::get(&platforms) != CL_SUCCESS){
        std::cout << "Searching platforms failed\n";
        exit(0);
    }

    //Printing the name of the platform used 
    std::cout << "Using platform: " << platforms[0].getInfo<CL_PLATFORM_NAME>() << "\nTotal platforms: " << platforms.size() << std::endl;
    
    // Select the default platform and create a context using this platform and the GPU
    //cl_context_properties cps[] ={CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
    //                                CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
     //                               CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(),
      //                              0};

    cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};

    context = new Context(CL_DEVICE_TYPE_GPU, cps);

    // Get a list of devices on this platform
    devices = context->getInfo<CL_CONTEXT_DEVICES>();

    //Printing the name of the default device used
    std::cout<< "Using device: " << devices[2].getInfo<CL_DEVICE_NAME>()<<"\nTotal devices: " << devices.size() << "\n";

    // Create a command queue and use the first device
    queue = new CommandQueue(*context, devices[2]);
}

//Creating the kernel
Kernel* OpenCL::createKernel(std::string kernelFile, std::string kernelName){
    // Read source file
    std::ifstream sourceFile(kernelFile.c_str());
    std::string sourceCode(
        std::istreambuf_iterator<char>(sourceFile),
        (std::istreambuf_iterator<char>()));
    Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

    // Make program of the source code in the context
    Program program = Program(*context, source);
    
    // Build program for these specific devices and spitting out errors
    if(program.build(devices, "-I structs/structures.h") != CL_SUCCESS){
        std::cout <<" Error building kernel: "<< kernelFile.c_str();
        std::cout << "\nLog:\n===================================\n";
        std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
        std::cout <<"\n========================================\n";
        exit(1);
    }

    // Make kernel
    Kernel* kernel = new Kernel(program, kernelName.c_str());

    //Also adding the kernel to the list of kernels
    addKernel(kernel, kernelName);

    return kernel;
}

//Adding the kernel to the map
void OpenCL::addKernel(Kernel* kernel, std::string kernelName){
    kernels.insert(std::pair<std::string, Kernel*>(kernelName, kernel));
}

//Running the kernel
void OpenCL::runKernel(Kernel* kernel, NDRange offset, NDRange globalRange, NDRange localRange){
    queue->enqueueNDRangeKernel(*kernel, offset, globalRange, localRange);
}

//Flushing the queue
void OpenCL::flushQueue(){
    queue->finish();
}
