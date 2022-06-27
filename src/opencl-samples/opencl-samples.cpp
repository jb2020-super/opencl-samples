// opencl-samples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>
#include <array>

#include "CL/cl.h"

#define CHECK_CL_ERROR(func) \
    error = func;\
    if (error < 0) {\
        std::cout << #func << " failed with: " << error << std::endl;\
        return 0;\
    }

int display_platform_info(cl_platform_id id, cl_platform_info param_name, const char* name)
{
    cl_int error{};
    size_t param_size{};

    CHECK_CL_ERROR(clGetPlatformInfo(id, param_name, 0, nullptr, &param_size));    
    char* info = (char*)alloca(sizeof(char) * param_size);
    CHECK_CL_ERROR(clGetPlatformInfo(id, param_name, param_size, info, nullptr));
    std::cout << name << ": " << info << std::endl;
}

int display_device_detail(cl_device_id id, cl_device_info param_name, const char* name)
{
    cl_int error{};
    size_t param_size{};
    CHECK_CL_ERROR(clGetDeviceInfo(id, param_name, 0, nullptr, &param_size));
    long long llval{};
    char* val{};
    switch (param_size)
    {
    case 8:
    case 4:
        CHECK_CL_ERROR(clGetDeviceInfo(id, param_name, param_size, &llval, nullptr));
        break;
    default:
        val = (char*)alloca(param_size);
        CHECK_CL_ERROR(clGetDeviceInfo(id, param_name, param_size, val, nullptr));
        break;
    }
    switch (param_name)
    {
    case CL_DEVICE_TYPE:
    {
        switch (llval)
        {
        case CL_DEVICE_TYPE_CPU:
            std::cout << "CPU detected\n";
            break;
        case CL_DEVICE_TYPE_GPU:
            std::cout << "GPU detected\n";
            break;
        case CL_DEVICE_TYPE_DEFAULT:
            std::cout << "Default detected\n";
            break;
        default:
            break;
        }
        break;
    }
    case CL_DEVICE_MAX_WORK_ITEM_SIZES:
    {
        if (param_size >= 8) {
            std::cout << name << ": ";
            long long* pval = reinterpret_cast<long long*>(val);
            for (int i = 0; i < param_size / 8; ++i) {
                std::cout << pval[i] << ", ";
            }
            std::cout << std::endl;
        }
        break;
    }
    default:
        std::cout << name << ": " << llval << std::endl;
        break;
    }
    
}

int display_device_info(cl_platform_id id, cl_device_type dev_type)
{
    cl_int error{};
    cl_uint dev_cnt{};
    CHECK_CL_ERROR(clGetDeviceIDs(id, dev_type, 0, nullptr, &dev_cnt));
    cl_device_id* devices = (cl_device_id*)alloca(sizeof(cl_device_id) * dev_cnt);
    CHECK_CL_ERROR(clGetDeviceIDs(id, dev_type, dev_cnt, devices, nullptr));
    std::cout << "Num of detected devices: " << dev_cnt << std::endl;
    for (cl_uint i = 0; i < dev_cnt; ++i){
        display_device_detail(devices[i], CL_DEVICE_TYPE, "CL_DEVICE_TYPE");
        display_device_detail(devices[i], CL_DEVICE_VENDOR_ID, "CL_DEVICE_VENDOR_ID");
        display_device_detail(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, "CL_DEVICE_MAX_COMPUTE_UNITS");
        display_device_detail(devices[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS");
        display_device_detail(devices[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, "CL_DEVICE_MAX_WORK_ITEM_SIZES");
        display_device_detail(devices[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, "CL_DEVICE_MAX_WORK_GROUP_SIZE");
    }
}
int main()
{
    //cl_platform_id* platforms{};
    cl_uint num_of_platforms{};
    cl_int error{};

    CHECK_CL_ERROR(clGetPlatformIDs(0, nullptr, &num_of_platforms));
    
    std::unique_ptr<cl_platform_id[]> platforms(new cl_platform_id[num_of_platforms]);
    
    CHECK_CL_ERROR(clGetPlatformIDs(num_of_platforms, platforms.get(), nullptr));
    
    for (cl_uint i = 0; i < num_of_platforms; ++i) {
        display_platform_info(platforms[i], CL_PLATFORM_PROFILE, "CL_PLATFORM_PROFILE");

        display_platform_info(platforms[i], CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION");

        display_platform_info(platforms[i], CL_PLATFORM_NAME, "CL_PLATFORM_NAME");

        display_platform_info(platforms[i], CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");

        display_platform_info(platforms[i], CL_PLATFORM_EXTENSIONS, "CL_PLATFORM_EXTENSIONS");
        
        display_device_info(platforms[i], CL_DEVICE_TYPE_CPU);

        display_device_info(platforms[i], CL_DEVICE_TYPE_GPU);

        //display_device_info(platforms[i], CL_DEVICE_TYPE_DEFAULT);
    }
    
    return 0;
}

