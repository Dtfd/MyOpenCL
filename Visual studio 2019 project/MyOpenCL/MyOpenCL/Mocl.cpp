#include "Mocl.h"

namespace MyOpenCL
{
	bool Mocl::initialized = false;

	cl_int Mocl::error = 0;

	size_t Mocl::localWorkGroupSize = 0;
	size_t Mocl::localWorkGroupSizeSqrt = 0;
	size_t Mocl::localWorkGroupSizeCbrt = 0;

	cl_platform_id Mocl::platform = NULL;
	cl_device_id Mocl::device = NULL;
	cl_context Mocl::context = NULL;
	cl_command_queue Mocl::queue = NULL;

	std::map<const char*, cl_program> Mocl::programs = std::map<const char*, cl_program>();
	std::map<const char*, cl_kernel>  Mocl::kernels = std::map<const char*, cl_kernel>();

	void Mocl::Initialize(cl_device_type clDeviceType)
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
			localWorkGroupSizeSqrt = PreviousPowerOfTwo((size_t)sqrt(localWorkGroupSize));
			localWorkGroupSizeCbrt = PreviousPowerOfTwo((size_t)cbrt(localWorkGroupSize));
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

	void Mocl::Destroy()
	{
		if (initialized == false)
		{
			error = MOCL_NOT_INITIALIZED;
			return;
		}
		for (std::map<const char*, cl_kernel>::iterator it = kernels.begin(); it != kernels.end(); it++)
		{
			error = clReleaseKernel(kernels[it->first]);
			MOCL_CHECK_ERROR_("clReleaseKernel(" << it->second << ")");
		}
		for (std::map<const char*, cl_program>::iterator it = programs.begin(); it != programs.end(); it++)
		{
			error = clReleaseProgram(programs[it->first]);
			MOCL_CHECK_ERROR_("clReleaseProgram(" << it->second << ")");
		}
		error = clReleaseCommandQueue(queue);
		MOCL_CHECK_ERROR_("clReleaseCommandQueue", );
		error = clReleaseContext(context);
		MOCL_CHECK_ERROR_("clReleaseContext", );
		error = clReleaseDevice(device);
		MOCL_CHECK_ERROR_("clReleaseDevice", );

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

	cl_int Mocl::Error()
	{
		return error;
	}

	size_t Mocl::RoundToMaxWorkGroupSizeMultiple(int n)
	{
		return n + (localWorkGroupSize - n % localWorkGroupSize);
	}

	size_t Mocl::RoundToMaxWorkGroupSizeSqrtMultiple(int n)
	{
		return n + (localWorkGroupSizeSqrt - n % localWorkGroupSizeSqrt);
	}


	size_t Mocl::RoundToMaxWorkGroupSizeCbrtMultiple(int n)
	{
		return n + (localWorkGroupSizeCbrt - n % localWorkGroupSizeCbrt);
	}

	size_t Mocl::PreviousPowerOfTwo(size_t x)
	{
		if (x == 0) return 0;
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		return x - (x >> 1);
	}

	void Mocl::Test()
	{
		
		const char* progS = 
			"__kernel void Add(__global int*a, __global int* b,__global int* n)\n"
			"{\n"
			"	int i = get_global_id(0);\n"
			"	if(i >= n[0]) return;\n"
			"	a[i] += b[i];\n"
			"}";
		
		size_t progSL = strlen(progS);
		cl_program prog = clCreateProgramWithSource(context, 1, &progS,&progSL, &error);
		MOCL_CHECK_ERROR_("clCreateProgramWithSource", return; );
		
		error = clBuildProgram(prog, 1, &device, "", NULL, NULL);
		if (error != CL_SUCCESS) {
			std::string str;
			cl::Program(prog).getBuildInfo<std::string>(cl::Device(device), CL_PROGRAM_BUILD_LOG, &str);
			std::cout << str << std::endl;
		}
		
		programs.insert({ "P1",prog });
		
		cl_kernel kern = clCreateKernel(prog, "Add", &error);
		MOCL_CHECK_ERROR_("clCreateKernel", return; );
		
		kernels.insert({ "Add",kern });
		int N = 100000;
		int* a = (int*) malloc(N * sizeof(int));
		int* b = (int*) malloc(N * sizeof(int));
		for (size_t i = 0; i < N; i++)
		{
			a[i] = i;
			b[i] = N - i;
		}
		
		cl_mem mA = clCreateBuffer(context, CL_MEM_READ_WRITE, N * sizeof(int), NULL, &error);
		MOCL_CHECK_ERROR_("clCreateBuffer(a)", return; );
		
		cl_mem mB = clCreateBuffer(context, CL_MEM_READ_WRITE, N * sizeof(int), NULL, &error);
		MOCL_CHECK_ERROR_("clCreateBuffer(b)", return; );
		
		cl_mem mN = clCreateBuffer(context, CL_MEM_READ_WRITE,sizeof(int), NULL, &error);
		MOCL_CHECK_ERROR_("clCreateBuffer(N)", return; );
		

		error = clSetKernelArg(kernels["Add"], 0, sizeof(cl_mem), &mA);
		MOCL_CHECK_ERROR_("clSetKernelArg(a)", return; );
		

		error = clSetKernelArg(kernels["Add"], 1, sizeof(cl_mem), &mB);
		MOCL_CHECK_ERROR_("clSetKernelArg(b)", return; );
		

		error = clSetKernelArg(kernels["Add"], 2, sizeof(cl_mem), &mN);
		MOCL_CHECK_ERROR_("clSetKernelArg(N)", return; );
		

		cl_event evWrite[3];
		error = clEnqueueWriteBuffer(queue,mA, CL_TRUE, NULL, N * sizeof(int), a, NULL, NULL, evWrite);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(a)", return; );
		
		error = clEnqueueWriteBuffer(queue,mB, CL_TRUE, NULL, N * sizeof(int), b, NULL, NULL, evWrite+1);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(b)", return; );
		
		error = clEnqueueWriteBuffer(queue,mN, CL_TRUE, NULL,sizeof(int), &N, NULL, NULL, evWrite+2);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(c)", return; );
		


		size_t gWG = RoundToMaxWorkGroupSizeMultiple(N);
		cl_event evND;
		
		error = clEnqueueNDRangeKernel(queue, kernels["Add"], 1, NULL, &gWG, &localWorkGroupSize, 3, evWrite, &evND);
		MOCL_CHECK_ERROR_("clEnqueueNDRangeKernel(Add)", return; );
		
		cl_event evRead;
		error = clEnqueueReadBuffer(queue, mA, CL_TRUE, NULL, N * sizeof(int), a, 1, &evND, &evRead);
		MOCL_CHECK_ERROR_("clEnqueueReadBuffer(a)", return; );
		
		error = clWaitForEvents(1, &evRead);
		MOCL_CHECK_ERROR_("clWaitForEvents(evRead)", return; );
		
		for (size_t i = 0; i < N; i++)
		{
			if (a[i] != N) std::cout << i << " | " << a[i] << std::endl; break;
		}

		
		free(a);
		free(b);
		clReleaseMemObject(mA);
		clReleaseMemObject(mB);
		clReleaseMemObject(mN);

		clReleaseEvent(evND);
		clReleaseEvent(evWrite[0]);
		clReleaseEvent(evWrite[1]);
		clReleaseEvent(evWrite[2]);
		clReleaseEvent(evRead);
	}

}