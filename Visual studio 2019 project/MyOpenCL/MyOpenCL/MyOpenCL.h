#pragma once
#include <CL/cl2.hpp>

#include <iostream>
/*
#ifndef MY_OPENCL_ECHO_OFF
#define MY_OPEN_CL_ECHO(msg) std::cout << msg
#define MY_OPEN_CL_ECHO_L(msg) std::cout << msg << std::endl;
#else
#define MY_OPEN_CL_ECHO(msg)
#define MY_OPEN_CL_ECHO_L(msg) 
#endif // !MY_OPENCL_ECHO_OFF
*/

#ifndef MY_OPENCL_ECHO_ERROR_OFF
#define MY_OPEN_CL_CHECK_ERROR(errorCode, msg, action) if(errorCode != CL_SUCCESS){ std::cout << msg << ": " << errorCode << std::endl; action }
#define MY_OPEN_CL_CHECK_ERROR_(msg, action) if(error != CL_SUCCESS){ std::cout << msg << ": " << error << std::endl; action }
#else
#define MY_OPEN_CL_CHECK_ERROR(errorCode, msg, action) if(errorCode != CL_SUCCESS){ action }
#define MY_OPEN_CL_CHECK_ERROR_(msg, action) if(error != CL_SUCCESS){ action }
#endif // MOCL_ECHO



class MyOpenCL
{
/*Variables*/
protected:
	cl_int error;

	size_t localWorkGroupSize;
	size_t localWorkGroupSizeSqrt;
	size_t localWorkGroupSizeCbrt;

	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;

	std::vector<cl_program> programs;
	std::vector<cl_kernel>  kernels;
/*Functions*/
public:
	MyOpenCL(cl_device_type clDeviceType = CL_DEVICE_TYPE_GPU);
	~MyOpenCL();
	cl_int Error();
};