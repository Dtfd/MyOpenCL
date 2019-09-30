#pragma once
#include <CL/cl2.hpp>

#include <fstream>
#include <sstream>
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
#define MOCL_PROGRAM_NOT_LOADED 3
#define MOCL_KERNEL_NOT_LOADED 4
#define MOCL_PATH_NOT_FOUND 5

#define MOCL_ASSERT_INITIALIZED if(Mocl::initialized != true){error = MOCL_NOT_INITIALIZED; return;}
#define MOCL_ASSERT_PROGRAM_LOADED(programName) if(programs.count(programName) != 1){error = MOCL_PROGRAM_NOT_LOADED; return;}
#define MOCL_ASSERT_KERNEL_LOADED(kernelName) if(kernels.count(kernelName) != 1){error = MOCL_KERNEL_NOT_LOADED; return;}

namespace MyOpenCL
{
	static class Mocl
	{
		/*Variables*/
	protected:
		static bool initialized;
		static cl_int error;

		static size_t localWorkGroupSize;
		static size_t* localWorkGroupSizeSqrt;
		static size_t* localWorkGroupSizeCbrt;

		static cl_platform_id platform;
		static cl_device_id device;
		static cl_context context;
		static cl_command_queue queue;

		static std::map<std::string, cl_program> programs;
		static std::map<std::string, cl_kernel>  kernels;

		/*Functions*/
	public:
		static void Initialize(cl_device_type clDeviceType = CL_DEVICE_TYPE_GPU);
		static void Destroy();
		static cl_int Error();
		/*Debug testing*/
		static void Test();
		static void Test2();
		static void Test3();
	protected:

		static size_t RoundToMaxWorkGroupSizeMultiple(int n);
		static size_t RoundToMaxWorkGroupSizeSqrtMultiple(int n);
		static size_t RoundToMaxWorkGroupSizeCbrtMultiple(int n);
		static size_t PreviousPowerOfTwo(size_t x);

		static void AddAndBuildProgramWithSource(std::string programName, 
												 std::vector<std::string> sources,
												 const char* builOptions);

		static void AddKernel(std::string kernelName, 
							  std::string programName);

		static void AddKernels(std::vector<std::string> kernelNames, 
							  std::string programName);

		static void AddKernel(std::string kernelName, 
							  std::string programName, 
							  const char* programBuildOptions, 
							  std::string source);

		static void AddKernels(std::vector<std::string> kernelNames,
							   std::string programName, 
							   const char* programBuildOptions, 
							   std::vector<std::string> sources);

		static void CreateBuffer(cl_mem* mem, 
							     cl_mem_flags flags,
							     size_t size,
							     void* hostPtr);

		static void SetKernelArg(std::string kernelName,
								 cl_uint argIndex, 
								 size_t argSize, 
								 const void* argValue);

		static void EnqueueWriteBuffer(cl_mem* buffer, 
									   cl_bool blockingWrite, 
									   size_t offset, 
									   size_t size, 
									   const void* ptr,
									   cl_uint num_events_in_wait_list, 
									   const cl_event* event_wait_list, 
									   cl_event* event);

		static void EnqueueReadBuffer(cl_mem* buffer, 
									  cl_bool blockingRead, size_t offset, 
									  size_t size, 
									  void* ptr, 
									  cl_uint num_events_in_wait_list, 
									  const cl_event* event_wait_list, 
									  cl_event* event);

		static void EnqueueNDRangeKernel(std::string kernelName,
										 cl_uint work_dim,
										 const size_t* global_work_offset,
										 const size_t* global_work_size,
										 const size_t* local_work_size,
										 cl_uint num_events_in_wait_list,
										 const cl_event* event_wait_list,
										 cl_event* event);

		static void Enqueue1DRangeKernel(std::string kernelName,
										 const size_t* global_work_offset,
										 size_t* global_work_size,
										 cl_uint num_events_in_wait_list,
										 const cl_event* event_wait_list,
										 cl_event* event);

		static void Enqueue2DRangeKernel(std::string kernelName,
										 const size_t* global_work_offset,
										 size_t* global_work_size,
										 cl_uint num_events_in_wait_list,
										 const cl_event* event_wait_list,
										 cl_event* event);

		static void Enqueue3DRangeKernel(std::string kernelName,
										 const size_t* global_work_offset,
										 size_t* global_work_size,
										 cl_uint num_events_in_wait_list,
										 const cl_event* event_wait_list,
										 cl_event* event);
		static void WaitForEvents(cl_uint numEvents,
								  cl_event* eventList);

		static void WaitForEvents(cl_event* event);
	};

}
