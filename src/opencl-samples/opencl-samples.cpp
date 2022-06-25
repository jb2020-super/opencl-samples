// opencl-samples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>
#include <array>

#include "CL/cl.h"

void display_platform_info(cl_platform_id id, cl_platform_info param_name, const char* name)
{
    cl_int error{};
    size_t param_size{};

    error = clGetPlatformInfo(id, param_name, 0, nullptr, &param_size);
    if (error < 0) {
        std::cout << "clGetPlatformInfo failed with: " << error << std::endl;
    }
    char* info = (char*)alloca(sizeof(char) * param_size);
    error = clGetPlatformInfo(id, param_name, param_size, info, nullptr);
    if (error != CL_SUCCESS) {
        std::cout << "clGetPlatformInfo failed with: " << error << std::endl;
    }
    std::cout << name << ": " << info << std::endl;
}

int main()
{
    //cl_platform_id* platforms{};
    cl_uint num_of_platforms{};
    cl_int error{};

    error = clGetPlatformIDs(0, nullptr, &num_of_platforms);
    if (error < 0) {
        std::cout << "OpenCL platform not found.\n";
        return 0;
    }
    
    std::unique_ptr<cl_platform_id[]> platforms(new cl_platform_id[num_of_platforms]);
    
    error = clGetPlatformIDs(num_of_platforms, platforms.get(), nullptr);
    if (error < 0) {
        std::cout << "clGetPlatformIDs failed with: " << error << std::endl;
        return 0;
    }
    for (cl_uint i = 0; i < num_of_platforms; ++i) {
        display_platform_info(platforms[i], CL_PLATFORM_PROFILE, "CL_PLATFORM_PROFILE");

        display_platform_info(platforms[i], CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION");

        display_platform_info(platforms[i], CL_PLATFORM_NAME, "CL_PLATFORM_NAME");

        display_platform_info(platforms[i], CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");

        display_platform_info(platforms[i], CL_PLATFORM_EXTENSIONS, "CL_PLATFORM_EXTENSIONS");
    }
    return 0;
}

