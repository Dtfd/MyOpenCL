#pragma once
#include <CL/cl2.hpp>

#include <iostream>
#include <map>
#ifndef MY_OPENCL_ECHO_ERROR_OFF
#define MOCL_CHECK_ERROR(errorCode, msg, action) if(errorCode != CL_SUCCESS){ std::cout << msg << ": " << errorCode << std::endl; action }
#define MOCL_CHECK_ERROR_(msg, action) if(error != CL_SUCCESS){ std::cout << msg << ": " << error << std::endl; action }
#else
#define MOCL_CHECK_ERROR(errorCode, msg, action) if(errorCode != CL_SUCCESS){ action }
#define MOCL_CHECK_ERROR_(msg, action) if(error != CL_SUCCESS){ action }
#endif // MOCL_ECHO


#define MOCL_NOT_INITIALIZED 1
#define MOCL_ALREADY_INITIALIZED 2 

#define MOCL_ASSERT_INITIALIZED if(Mocl::initialized != true){error = MOCL_NOT_INITIALIZED; return;}

namespace MyOpenCL
{
	static class Mocl
	{
		/*Variables*/
	protected:
		static bool initialized;
		static cl_int error;

		static size_t localWorkGroupSize;
		static size_t localWorkGroupSizeSqrt;
		static size_t localWorkGroupSizeCbrt;

		static cl_platform_id platform;
		static cl_device_id device;
		static cl_context context;
		static cl_command_queue queue;

		static std::map<const char*, cl_program> programs;
		static std::map<const char*, cl_kernel>  kernels;
		/*Functions*/
	public:
		static void Initialize(cl_device_type clDeviceType = CL_DEVICE_TYPE_GPU);
		static void Destroy();
		static cl_int Error();

		static size_t RoundToMaxWorkGroupSizeMultiple(int n);
		static size_t RoundToMaxWorkGroupSizeSqrtMultiple(int n);
		static size_t RoundToMaxWorkGroupSizeCbrtMultiple(int n); 
		static size_t PreviousPowerOfTwo(size_t x);
		static void Test();
	protected:
		
	};

}
