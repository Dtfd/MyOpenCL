#include "MyOpenCL.h"

MyOpenCL::MyOpenCL(cl_device_type clDeviceType)
{
	MY_OPEN_CL_ECHO_L("Create instance of MyOpenCL:");
	device = NULL;
	cl_device_id deviceCPU = NULL;
	cl_uint sizeAllPlatforms = 10;
	cl_uint numAllPlatforms;
	cl_platform_id* allPlatforms = (cl_platform_id*) malloc(sizeAllPlatforms *sizeof(cl_platform_id));
	MY_OPEN_CL_ECHO("\tGet all platforms: ");
	error = clGetPlatformIDs(sizeAllPlatforms, allPlatforms, &numAllPlatforms);
	MY_OPEN_CL_CHECK_ERROR_("clGetPlatformIDs", )
	else
	{
		MY_OPEN_CL_ECHO_L("Success");
		MY_OPEN_CL_ECHO_L("\tGet device (type: " << clDeviceType << "): ");
		for (size_t i = 0; i < numAllPlatforms; i++)
		{
			MY_OPEN_CL_ECHO("\t\tplatform "<<i<<": ");
			error = clGetDeviceIDs(allPlatforms[i], clDeviceType, 1, &device, NULL);
			MY_OPEN_CL_CHECK_ERROR_("clGetDeviceIDs", device = NULL;)
			else break;
			if (deviceCPU == NULL) 
			{
				error = clGetDeviceIDs(allPlatforms[i], CL_DEVICE_TYPE_CPU, 1, &deviceCPU, NULL);
				if (error != CL_SUCCESS) deviceCPU = NULL;
			}
		}
		if (device == NULL) 
		{
			MY_OPEN_CL_ECHO("\tGet default CPU: ");
			if (deviceCPU != NULL)
			{
				MY_OPEN_CL_ECHO_L("Success");
				device = deviceCPU;
				error = CL_SUCCESS;
			}
		}else MY_OPEN_CL_ECHO_L("Success");

		MY_OPEN_CL_CHECK_ERROR_("clGetDeviceIDs", )
		else 
		{
			MY_OPEN_CL_ECHO("\tGet max work group size: ");
			error = cl::Device(device).getInfo<size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE, &localWorkGroupSize);
			MY_OPEN_CL_CHECK_ERROR_("cl::Device(device).getInfo", )
			else
			{
				MY_OPEN_CL_ECHO_L("Success");
				localWorkGroupSizeSqrt = sqrt(localWorkGroupSize);
				localWorkGroupSizeCbrt = cbrt(localWorkGroupSize);

				MY_OPEN_CL_ECHO("\tCreate context: ");
				context = clCreateContext(NULL, 1, &device, NULL, NULL, &error);
				MY_OPEN_CL_CHECK_ERROR_("clCreateContext", context = NULL;)
				else
				{
					MY_OPEN_CL_ECHO_L("Success");
					MY_OPEN_CL_ECHO("\tCreate command queue: ");
					cl_command_queue_properties cqp = CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
					#pragma warning(suppress : 4996)
					queue = clCreateCommandQueue(context, device, cqp, &error);
					MY_OPEN_CL_CHECK_ERROR_("clCreateCommandQueue", queue = NULL;)
					else 
					{
						MY_OPEN_CL_ECHO_L("Success");
					}
				}
			}
		}
	}
	/*Clean up*/
	free(allPlatforms);
	MY_OPEN_CL_CHECK_ERROR_("Failed to create MyOpenCL instance", )
	else MY_OPEN_CL_ECHO_L("Instance created successfully");
}

MyOpenCL::~MyOpenCL()
{
}

cl_int MyOpenCL::Error()
{
	return cl_int();
}
