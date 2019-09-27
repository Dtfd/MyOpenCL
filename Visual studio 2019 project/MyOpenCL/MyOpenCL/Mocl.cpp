#include "MyOpenCL.h"
bool MyOpenCL::initialized = false;

cl_int MyOpenCL::error = 0;

size_t MyOpenCL::localWorkGroupSize = 0;
size_t MyOpenCL::localWorkGroupSizeSqrt = 0;
size_t MyOpenCL::localWorkGroupSizeCbrt = 0;

cl_platform_id MyOpenCL::platform = NULL;
cl_device_id MyOpenCL::device = NULL;
cl_context MyOpenCL::context = NULL;
cl_command_queue MyOpenCL::queue = NULL;

std::map<const char*, cl_program> MyOpenCL::programs = std::map<const char*, cl_program>();
std::map<const char*, cl_kernel>  MyOpenCL::kernels = std::map<const char*, cl_kernel>();

void MyOpenCL::Initialize(cl_device_type clDeviceType)
{
	if (initialized == true) 
	{
		error = MOCL_ALREADY_INITIALIZED;
		return;
	}
	device = NULL;
	cl_device_id deviceCPU = NULL;
	cl_uint sizeAllPlatforms = 10;
	cl_uint numAllPlatforms;
	cl_platform_id* allPlatforms;

	/*Get all platforms*/
	allPlatforms = (cl_platform_id*)malloc(sizeAllPlatforms * sizeof(cl_platform_id));
	error = clGetPlatformIDs(sizeAllPlatforms, allPlatforms, &numAllPlatforms);
	MOCL_CHECK_ERROR_("clGetPlatformIDs", );

	/*Get default device of specified type, in case of fail select default CPU*/
	if (error == CL_SUCCESS)
	{
		for (size_t i = 0; i < numAllPlatforms; i++)
		{
			error = clGetDeviceIDs(allPlatforms[i], clDeviceType, 1, &device, NULL);
			if (error != CL_SUCCESS) { device = NULL; }
			else { break; }
			if (deviceCPU == NULL) 
			{
				error = clGetDeviceIDs(allPlatforms[i], CL_DEVICE_TYPE_CPU, 1, &deviceCPU, NULL);
				if (error != CL_SUCCESS) { deviceCPU = NULL; }
			}
		}
		if (device == NULL && deviceCPU != NULL)
		{
			device = deviceCPU;

			error = CL_SUCCESS;
		}
		else if (deviceCPU != NULL)
		{
			error = clReleaseDevice(deviceCPU);
			if (error != CL_SUCCESS) deviceCPU = NULL;
		}
		MOCL_CHECK_ERROR_("clGetDeviceIDs", );
	}

	/*Get max local work group size for dimentions 1-3*/
	if (error == CL_SUCCESS)
	{
		error = cl::Device(device).getInfo<size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE, &localWorkGroupSize);
		MOCL_CHECK_ERROR_("cl::Device(device).getInfo", );
		localWorkGroupSizeSqrt = sqrt(localWorkGroupSize);
		localWorkGroupSizeCbrt = cbrt(localWorkGroupSize);
	}
	/*Create context*/
	if (error == CL_SUCCESS)
	{
		context = clCreateContext(NULL, 1, &device, NULL, NULL, &error);
		MOCL_CHECK_ERROR_("clCreateContext", context = NULL;);
	}
	/*Create command queue*/
	if (error == CL_SUCCESS)
	{
		cl_command_queue_properties cqp = CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
#pragma warning(suppress : 4996)
		queue = clCreateCommandQueue(context, device, cqp, &error);
		MOCL_CHECK_ERROR_("clCreateCommandQueue", queue = NULL;);
	}

	//Clean
	free(allPlatforms);
	initialized = true;
}

void MyOpenCL::Destroy()
{
	if (initialized == false) 
	{
		error = MOCL_NOT_INITIALIZED;
		return;
	}
	for (std::map<const char*,cl_kernel>::iterator it = kernels.begin(); it != kernels.end(); it++)
	{
		clReleaseKernel(kernels[it->first]);
	}
	for (std::map<const char*, cl_program>::iterator it = programs.begin(); it != programs.end(); it++)
	{
		clReleaseProgram(programs[it->first]);
	}
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseDevice(device);

	error = 0;
	localWorkGroupSize = 0;
	localWorkGroupSizeSqrt = 0;
	localWorkGroupSizeCbrt = 0;

	platform = NULL;
	device = NULL;
	context = NULL;
	queue = NULL;

	programs.clear();
	kernels.clear();
	initialized = false;
}

cl_int MyOpenCL::Error()
{
	return error;
}

size_t MyOpenCL::RoundToMaxWorkGroupSizeMultiple(int n)
{
	return n + (localWorkGroupSize - n % localWorkGroupSize);
}

size_t MyOpenCL::RoundToMaxWorkGroupSizeSqrtMultiple(int n)
{
	return n + (localWorkGroupSizeSqrt - n % localWorkGroupSizeSqrt);
}


size_t MyOpenCL::RoundToMaxWorkGroupSizeCbrtMultiple(int n)
{
	return n + (localWorkGroupSizeCbrt - n % localWorkGroupSizeCbrt);
}
