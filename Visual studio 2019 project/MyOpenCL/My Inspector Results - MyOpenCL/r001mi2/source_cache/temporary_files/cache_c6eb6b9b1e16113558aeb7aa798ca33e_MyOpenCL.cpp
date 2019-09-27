#include "MyOpenCL.h"

MyOpenCL::MyOpenCL(cl_device_type clDeviceType)
{
	device = NULL;
	cl_device_id deviceCPU = NULL;
	cl_uint sizeAllPlatforms = 10;
	cl_uint numAllPlatforms;
	cl_platform_id* allPlatforms;

	/*Get all platforms*/
	allPlatforms = (cl_platform_id*)malloc(sizeAllPlatforms * sizeof(cl_platform_id));
	error = clGetPlatformIDs(sizeAllPlatforms, allPlatforms, &numAllPlatforms);
	MY_OPEN_CL_CHECK_ERROR_("clGetPlatformIDs", );

	/*Get default device of specified type, in case of fail select default CPU*/
	if (error == CL_SUCCESS)
	{
		for (size_t i = 0; i < numAllPlatforms; i++)
		{
			error = clGetDeviceIDs(allPlatforms[i], clDeviceType, 1, &device, NULL);
			if (error != CL_SUCCESS) { device = NULL; }
			else break;
			if (deviceCPU == NULL) 
			{
				error = clGetDeviceIDs(allPlatforms[i], CL_DEVICE_TYPE_CPU, 1, &deviceCPU, NULL);
				if (error != CL_SUCCESS) deviceCPU = NULL;
			}
		}
		if (device == NULL && deviceCPU != NULL)
		{
			device = deviceCPU;
			error = CL_SUCCESS;
		}
		MY_OPEN_CL_CHECK_ERROR_("clGetDeviceIDs", );
	}
	free(allPlatforms);
	
	/*Get max local work group size for dimentions 1-3*/
	if (error == CL_SUCCESS)
	{
		error = cl::Device(device).getInfo<size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE, &localWorkGroupSize);
		MY_OPEN_CL_CHECK_ERROR_("cl::Device(device).getInfo", );
		localWorkGroupSizeSqrt = sqrt(localWorkGroupSize);
		localWorkGroupSizeCbrt = cbrt(localWorkGroupSize);
	}
	/*Create context*/
	if (error == CL_SUCCESS)
	{
		context = clCreateContext(NULL, 1, &device, NULL, NULL, &error);
		MY_OPEN_CL_CHECK_ERROR_("clCreateContext", context = NULL;);
	}
	/*Create command queue*/
	if (error == CL_SUCCESS)
	{
		cl_command_queue_properties cqp = CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
		#pragma warning(suppress : 4996)
		queue = clCreateCommandQueue(context, device, cqp, &error);
		MY_OPEN_CL_CHECK_ERROR_("clCreateCommandQueue", queue = NULL;);
	}

	/*Clean up*/
}

MyOpenCL::~MyOpenCL()
{
	for (size_t i = 0; i < kernels.size(); i++)
	{
		error = clReleaseKernel(kernels[i]);
		MY_OPEN_CL_CHECK_ERROR_("clReleaseKernel(" << i << ")", );
	}
	for (size_t i = 0; i < programs.size(); i++)
	{
		error = clReleaseProgram(programs[i]);
		MY_OPEN_CL_CHECK_ERROR_("clReleaseProgram(" << i <<")", );
	}
}

cl_int MyOpenCL::Error()
{
	return error;
}
