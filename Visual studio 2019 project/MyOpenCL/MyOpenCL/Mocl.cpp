#include "Mocl.h"

namespace MyOpenCL
{
	bool Mocl::initialized = false;

	cl_int Mocl::error = 0;

	size_t Mocl::localWorkGroupSize = 0;
	size_t* Mocl::localWorkGroupSizeSqrt = nullptr;
	size_t* Mocl::localWorkGroupSizeCbrt = nullptr;


	cl_platform_id Mocl::platform = NULL;
	cl_device_id Mocl::device = NULL;
	cl_context Mocl::context = NULL;
	cl_command_queue Mocl::queue = NULL;

	std::map<std::string, cl_program> Mocl::programs = std::map<std::string, cl_program>();
	std::map<std::string, cl_kernel>  Mocl::kernels = std::map<std::string, cl_kernel>();

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
			localWorkGroupSizeSqrt = (size_t*)malloc(2 * sizeof(size_t));
			localWorkGroupSizeCbrt = (size_t*)malloc(3 * sizeof(size_t));
			error = cl::Device(device).getInfo<size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE, &localWorkGroupSize);
			MOCL_CHECK_ERROR_("cl::Device(device).getInfo", );
			localWorkGroupSizeSqrt[0] = PreviousPowerOfTwo((size_t)sqrt(localWorkGroupSize));
			localWorkGroupSizeSqrt[1] = PreviousPowerOfTwo((size_t)sqrt(localWorkGroupSize));


			localWorkGroupSizeCbrt[0] = PreviousPowerOfTwo((size_t)cbrt(localWorkGroupSize));
			localWorkGroupSizeCbrt[1] = PreviousPowerOfTwo((size_t)cbrt(localWorkGroupSize));
			localWorkGroupSizeCbrt[2] = PreviousPowerOfTwo((size_t)cbrt(localWorkGroupSize));
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

		MOCL_CHECK_ERROR_("LoadAndCreateKernel",)
		//Clean
		free(allPlatforms);
		initialized = true;
	}

	void Mocl::Destroy()
	{
		MOCL_ASSERT_INITIALIZED;
		for (std::map<std::string, cl_kernel>::iterator it = kernels.begin(); it != kernels.end(); it++)
		{
			error = clReleaseKernel(kernels[it->first]);
			MOCL_CHECK_ERROR_("clReleaseKernel(" << it->second << ")");
		}
		for (std::map<std::string, cl_program>::iterator it = programs.begin(); it != programs.end(); it++)
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
		free(localWorkGroupSizeSqrt);
		localWorkGroupSizeSqrt = nullptr;
		free(localWorkGroupSizeCbrt);
		localWorkGroupSizeCbrt = nullptr;

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

	void Mocl::OverrideLocalWorkGroupSize(int n)
	{
		localWorkGroupSize = n;

		localWorkGroupSizeSqrt[0] = PreviousPowerOfTwo((size_t)sqrt(localWorkGroupSize));
		localWorkGroupSizeSqrt[1] = PreviousPowerOfTwo((size_t)sqrt(localWorkGroupSize));

		localWorkGroupSizeCbrt[0] = PreviousPowerOfTwo((size_t)cbrt(localWorkGroupSize));
		localWorkGroupSizeCbrt[1] = PreviousPowerOfTwo((size_t)cbrt(localWorkGroupSize));
		localWorkGroupSizeCbrt[2] = PreviousPowerOfTwo((size_t)cbrt(localWorkGroupSize));
	}

	size_t Mocl::RoundToMaxWorkGroupSizeMultiple(int n)
	{
		return n + (localWorkGroupSize - n % localWorkGroupSize);
	}

	size_t Mocl::RoundToMaxWorkGroupSizeSqrtMultiple(int n)
	{
		return n + (localWorkGroupSizeSqrt[0] - (n % localWorkGroupSizeSqrt[0]));
	}

	size_t Mocl::RoundToMaxWorkGroupSizeCbrtMultiple(int n)
	{
		return n + (localWorkGroupSizeCbrt[0] - n % localWorkGroupSizeCbrt[0]);
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

	void Mocl::AddAndBuildProgramWithSource(std::string programName, std::vector<std::string> sources, const char* builOptions)
	{
		size_t sizeB;
		char** sourcesCh = (char**)malloc(sources.size() * sizeof(char*));
		size_t* sourceSizes = (size_t*)malloc(sources.size() * sizeof(size_t));
		for (size_t i = 0; i < sources.size(); i++)
		{
			sourceSizes[i] = sources[i].size();
			sizeB = sources[i].size() * sizeof(char);
			sourcesCh[i] = (char*)malloc(sizeB);
#pragma warning(suppress : 4996)
			strcpy(sourcesCh[i], sources[i].c_str());
		}
		programs[programName] = clCreateProgramWithSource(context, sources.size(),(const char**) sourcesCh, sourceSizes, &error);
		MOCL_CHECK_ERROR_("clCreateProgramWithSource(" << programName << ")", );
		if (error == CL_SUCCESS) 
		{
			error = clBuildProgram(programs[programName], 1, &device, builOptions, NULL, NULL);
			if (error != CL_SUCCESS) {
				std::string str;
				cl::Program(programs[programName]).getBuildInfo<std::string>(cl::Device(device), CL_PROGRAM_BUILD_LOG, &str);
				std::cout << str << std::endl;
			}
		}


		free(sourceSizes);

	}

	void Mocl::AddAndBuildProgramWithSource(std::vector<std::string> kernelNames, std::string programName, std::vector<std::string> sources, const char* builOptions)
	{
		AddAndBuildProgramWithSource(programName, sources, builOptions);
		MOCL_CHECK_ERROR_("AddAndBuildProgramWithSource",)
		CreateKernels(kernelNames, programName);
		MOCL_CHECK_ERROR_("AddAndBuildProgramWithSource", )
	}

	void Mocl::CreateKernel(std::string kernelName, std::string programName)
	{
		MOCL_ASSERT_PROGRAM_LOADED(programName);
		kernels[kernelName] = clCreateKernel(programs[programName],kernelName.c_str(), &error);
		MOCL_CHECK_ERROR_("clCreateKernel("<< kernelName <<")", );
	}

	void Mocl::CreateKernels(std::vector<std::string> kernelNames, std::string programName)
	{
		MOCL_ASSERT_PROGRAM_LOADED(programName);
		for (size_t i = 0; i < kernelNames.size(); i++)
		{
			kernels[kernelNames[i]] = clCreateKernel(programs[programName], kernelNames[i].c_str(), &error);
			MOCL_CHECK_ERROR_("clCreateKernel(" << kernelNames[i] << ")", );
		}
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
		cl_program prog = clCreateProgramWithSource(context, 1, &progS, &progSL, &error);
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
		int* a = (int*)malloc(N * sizeof(int));
		int* b = (int*)malloc(N * sizeof(int));
		for (size_t i = 0; i < N; i++)
		{
			a[i] = i;
			b[i] -= i;
		}

		cl_mem mA = clCreateBuffer(context, CL_MEM_READ_WRITE, N * sizeof(int), NULL, &error);
		MOCL_CHECK_ERROR_("clCreateBuffer(a)", return; );

		cl_mem mB = clCreateBuffer(context, CL_MEM_READ_WRITE, N * sizeof(int), NULL, &error);
		MOCL_CHECK_ERROR_("clCreateBuffer(b)", return; );

		cl_mem mN = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &error);
		MOCL_CHECK_ERROR_("clCreateBuffer(N)", return; );


		error = clSetKernelArg(kernels["Add"], 0, sizeof(cl_mem), &mA);
		MOCL_CHECK_ERROR_("clSetKernelArg(a)", return; );


		error = clSetKernelArg(kernels["Add"], 1, sizeof(cl_mem), &mB);
		MOCL_CHECK_ERROR_("clSetKernelArg(b)", return; );


		error = clSetKernelArg(kernels["Add"], 2, sizeof(cl_mem), &mN);
		MOCL_CHECK_ERROR_("clSetKernelArg(N)", return; );


		cl_event evWrite[3];
		error = clEnqueueWriteBuffer(queue, mA, CL_TRUE, NULL, N * sizeof(int), a, NULL, NULL, evWrite);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(a)", return; );

		error = clEnqueueWriteBuffer(queue, mB, CL_TRUE, NULL, N * sizeof(int), b, NULL, NULL, evWrite + 1);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(b)", return; );

		error = clEnqueueWriteBuffer(queue, mN, CL_TRUE, NULL, sizeof(int), &N, NULL, NULL, evWrite + 2);
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

	void Mocl::Test2()
	{
		cl_mem mA;
		cl_mem mB;
		cl_mem mN;

		const char** progS = (const char**)malloc(2 * sizeof(const char*));
		progS[0] =
			"__kernel void Add(__global int*a, __global int* b,__global int* n)\n"
			"{\n"
			"	int i = get_global_id(0);\n"
			"	if(i >= n[0]) return;\n"
			"	a[i] += b[i];\n"
			"}";
		progS[1] =
			"__kernel void Sub(__global int*a, __global int* b,__global int* n)\n"
			"{\n"
			"	int i = get_global_id(0);\n"
			"	if(i >= n[0]) return;\n"
			"	a[i] -= b[i];\n"
			"}";

		size_t progSL[2];
		progSL[0] = strlen(progS[0]);
		progSL[1] = strlen(progS[1]);
		programs.insert({ "P1",clCreateProgramWithSource(context, 2, progS,progSL, &error) });
		MOCL_CHECK_ERROR_("clCreateProgramWithSource", return; );

		error = clBuildProgram(programs["P1"], 1, &device, "", NULL, NULL);
		if (error != CL_SUCCESS) {
			std::string str;
			cl::Program(programs["P1"]).getBuildInfo<std::string>(cl::Device(device), CL_PROGRAM_BUILD_LOG, &str);
			std::cout << str << std::endl;
		}



		kernels.insert({ "Add",clCreateKernel(programs["P1"], "Add", &error) });
		MOCL_CHECK_ERROR_("clCreateKernel(Add)", return; );

		kernels.insert({ "Sub",clCreateKernel(programs["P1"], "Sub", &error) });
		MOCL_CHECK_ERROR_("clCreateKernel(Sub)", return; );

		int N = 100000;
		int* a = (int*)malloc(N * sizeof(int));
		int* b = (int*)malloc(N * sizeof(int));
		for (size_t i = 0; i < N; i++)
		{
			a[i] = i;
			b[i] = -1 * i;
		}

		CreateBuffer(&mA, CL_MEM_READ_WRITE, N * sizeof(int), NULL);
		MOCL_CHECK_ERROR_("clCreateBuffer(a)", return; );
		CreateBuffer(&mB, CL_MEM_READ_WRITE, N * sizeof(int), NULL);
		MOCL_CHECK_ERROR_("clCreateBuffer(b)", return; );
		CreateBuffer(&mN, CL_MEM_READ_WRITE, sizeof(int), NULL);
		MOCL_CHECK_ERROR_("clCreateBuffer(N)", return; );


		SetKernelArg("Sub", 0, sizeof(cl_mem), &mA);
		MOCL_CHECK_ERROR_("clSetKernelArg(a)", return; );
		SetKernelArg("Sub", 1, sizeof(cl_mem), &mB);
		MOCL_CHECK_ERROR_("clSetKernelArg(b)", return; );
		SetKernelArg("Sub", 2, sizeof(cl_mem), &mN);
		MOCL_CHECK_ERROR_("clSetKernelArg(N)", return; );


		cl_event evWrite[3];
		EnqueueWriteBuffer(&mA, CL_TRUE, NULL, N * sizeof(int), a, NULL, NULL, evWrite);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(a)", return; );

		EnqueueWriteBuffer(&mB, CL_TRUE, NULL, N * sizeof(int), b, NULL, NULL, evWrite + 1);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(b)", return; );

		EnqueueWriteBuffer(&mN, CL_TRUE, NULL, sizeof(int), &N, NULL, NULL, evWrite + 2);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(c)", return; );

		cl_event evND;
		size_t Nn = N;
		Enqueue1DRangeKernel("Sub", NULL, &Nn, 3, evWrite, &evND);
		MOCL_CHECK_ERROR_("clEnqueueNDRangeKernel(Sub)", return; );

		cl_event evRead;
		EnqueueReadBuffer(&mA, CL_TRUE, NULL, N * sizeof(int), a, 1, &evND, &evRead);
		MOCL_CHECK_ERROR_("clEnqueueReadBuffer(a)", return; );

		WaitForEvents(&evRead);
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

	void Mocl::Test3()
	{
		cl_mem mA;
		cl_mem mB;
		cl_mem mN;
		std::vector<std::string> sources;
		std::vector<std::string> kernelNames;
		kernelNames.push_back("Add");
		sources.push_back(
			"__kernel void Add(__global int*a, __global int* b,__global int* n)\n"
			"{\n"
			"	int i = get_global_id(0);\n"
			"	if(i >= n[0]) return;\n"
			"	a[i] += b[i];\n"
			"}"
		);
		kernelNames.push_back("Sub");
		sources.push_back(
			"__kernel void Sub(__global int*a, __global int* b,__global int* n)\n"
			"{\n"
			"	int i = get_global_id(0);\n"
			"	if(i >= n[0]) return;\n"
			"	a[i] -= b[i];\n"
			"}"
		);

		AddAndBuildProgramWithSource("P1", sources, "");

		CreateKernels(kernelNames, "P1");
		int N = 100000;
		int* a = (int*)malloc(N * sizeof(int));
		int* b = (int*)malloc(N * sizeof(int));
		for (size_t i = 0; i < N; i++)
		{
			a[i] = i;
			b[i] = -1 * i;
		}

		CreateBuffer(&mA, CL_MEM_READ_WRITE, N * sizeof(int), NULL);
		MOCL_CHECK_ERROR_("clCreateBuffer(a)", return; );
		CreateBuffer(&mB, CL_MEM_READ_WRITE, N * sizeof(int), NULL);
		MOCL_CHECK_ERROR_("clCreateBuffer(b)", return; );
		CreateBuffer(&mN, CL_MEM_READ_WRITE, sizeof(int), NULL);
		MOCL_CHECK_ERROR_("clCreateBuffer(N)", return; );


		SetKernelArg("Sub", 0, sizeof(cl_mem), &mA);
		MOCL_CHECK_ERROR_("clSetKernelArg(a)", return; );
		SetKernelArg("Sub", 1, sizeof(cl_mem), &mB);
		MOCL_CHECK_ERROR_("clSetKernelArg(b)", return; );
		SetKernelArg("Sub", 2, sizeof(cl_mem), &mN);
		MOCL_CHECK_ERROR_("clSetKernelArg(N)", return; );


		cl_event evWrite[3];
		EnqueueWriteBuffer(&mA, CL_TRUE, NULL, N * sizeof(int), a, NULL, NULL, evWrite);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(a)", return; );

		EnqueueWriteBuffer(&mB, CL_TRUE, NULL, N * sizeof(int), b, NULL, NULL, evWrite + 1);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(b)", return; );

		EnqueueWriteBuffer(&mN, CL_TRUE, NULL, sizeof(int), &N, NULL, NULL, evWrite + 2);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(c)", return; );

		cl_event evND;
		size_t Nn = N;
		Enqueue1DRangeKernel("Sub", NULL, &Nn, 3, evWrite, &evND);
		MOCL_CHECK_ERROR_("clEnqueueNDRangeKernel(Sub)", return; );

		cl_event evRead;
		EnqueueReadBuffer(&mA, CL_TRUE, NULL, N * sizeof(int), a, 1, &evND, &evRead);
		MOCL_CHECK_ERROR_("clEnqueueReadBuffer(a)", return; );

		WaitForEvents(&evRead);
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

	void Mocl::Test4()
	{
		cl_mem mA;
		cl_mem mB;
		cl_mem mN;
		std::vector<std::string> sources;
		std::vector<std::string> kernelNames;
		kernelNames.push_back("Add");
		sources.push_back(
			"__kernel void Add(__global int*a, __global int* b,__global int* n)\n"
			"{\n"
			"	int i = get_global_id(0);\n"
			"	if(i >= n[0]) return;\n"
			"	a[i] += b[i];\n"
			"}"
		);
		kernelNames.push_back("Sub");
		sources.push_back(
			"__kernel void Sub(__global int*a, __global int* b,__global int* n)\n"
			"{\n"
			"	int i = get_global_id(0);\n"
			"	if(i >= n[0]) return;\n"
			"	a[i] -= b[i];\n"
			"}"
		);

		AddAndBuildProgramWithSource(kernelNames, "P1", sources, "");
		MOCL_CHECK_ERROR_("AddAndBuildProgramWithSource", );

		int N = 10000000;
		int* a = (int*)malloc(N * sizeof(int));
		int* b = (int*)malloc(N * sizeof(int));
		for (size_t i = 0; i < N; i++)
		{
			a[i] = i;
			b[i] = -1 * i;
		}

		CreateBuffer(&mA, CL_MEM_READ_WRITE, N * sizeof(int), NULL);
		MOCL_CHECK_ERROR_("clCreateBuffer(a)", return; );
		CreateBuffer(&mB, CL_MEM_READ_WRITE, N * sizeof(int), NULL);
		MOCL_CHECK_ERROR_("clCreateBuffer(b)", return; );
		CreateBuffer(&mN, CL_MEM_READ_WRITE, sizeof(int), NULL);
		MOCL_CHECK_ERROR_("clCreateBuffer(N)", return; );


		SetKernelArg("Sub", 0, sizeof(cl_mem), &mA);
		MOCL_CHECK_ERROR_("clSetKernelArg(a)", return; );
		SetKernelArg("Sub", 1, sizeof(cl_mem), &mB);
		MOCL_CHECK_ERROR_("clSetKernelArg(b)", return; );
		SetKernelArg("Sub", 2, sizeof(cl_mem), &mN);
		MOCL_CHECK_ERROR_("clSetKernelArg(N)", return; );


		cl_event evWrite[3];
		EnqueueWriteBuffer(&mA, CL_TRUE, NULL, N * sizeof(int), a, NULL, NULL, evWrite);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(a)", return; );

		EnqueueWriteBuffer(&mB, CL_TRUE, NULL, N * sizeof(int), b, NULL, NULL, evWrite + 1);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(b)", return; );

		EnqueueWriteBuffer(&mN, CL_TRUE, NULL, sizeof(int), &N, NULL, NULL, evWrite + 2);
		MOCL_CHECK_ERROR_("clEnqueueWriteBuffer(c)", return; );

		cl_event evND;
		size_t Nn = N;
		Enqueue1DRangeKernel("Sub", NULL, &Nn, 3, evWrite, &evND);
		MOCL_CHECK_ERROR_("clEnqueueNDRangeKernel(Sub)", return; );

		cl_event evRead;
		EnqueueReadBuffer(&mA, CL_TRUE, NULL, N * sizeof(int), a, 1, &evND, &evRead);
		MOCL_CHECK_ERROR_("clEnqueueReadBuffer(a)", return; );

		WaitForEvents(&evRead);
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

	void Mocl::CreateBuffer(cl_mem* mem, cl_mem_flags flags, size_t size, void* hostPtr)
	{
		*mem = clCreateBuffer(context, flags, size, hostPtr, &error);
	}

	void Mocl::SetKernelArg(std::string kernelName, cl_uint argIndex, size_t argSize, const void* argValue)
	{
		error = clSetKernelArg(kernels[kernelName], argIndex, argSize, argValue);
	}

	void Mocl::EnqueueWriteBuffer(cl_mem* buffer, cl_bool blockingWrite, size_t offset, size_t size, const void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
	{
		error = clEnqueueWriteBuffer(queue, *buffer, blockingWrite, offset, size, ptr, num_events_in_wait_list, event_wait_list, event);
	}

	void Mocl::EnqueueReadBuffer(cl_mem* buffer, cl_bool blockingRead, size_t offset, size_t size, void* ptr, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
	{
		error = clEnqueueReadBuffer(queue, *buffer, blockingRead, offset, size, ptr, num_events_in_wait_list, event_wait_list, event);
	}

	void Mocl::EnqueueNDRangeKernel(std::string kernelName, cl_uint work_dim, const size_t* global_work_offset, const size_t* global_work_size, const size_t* local_work_size, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
	{
		error = clEnqueueNDRangeKernel(queue, kernels[kernelName], work_dim, global_work_offset, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);
	}

	void Mocl::Enqueue1DRangeKernel(std::string kernelName, const size_t* global_work_offset,  size_t* global_work_size, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
	{
		*global_work_size = RoundToMaxWorkGroupSizeMultiple(*global_work_size);
		error = clEnqueueNDRangeKernel(queue, kernels[kernelName], 1, global_work_offset, global_work_size, &localWorkGroupSize, num_events_in_wait_list, event_wait_list, event);
	}

	void Mocl::Enqueue2DRangeKernel(std::string kernelName, const size_t* global_work_offset, size_t* global_work_size, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
	{
		global_work_size[0] = RoundToMaxWorkGroupSizeSqrtMultiple(*global_work_size);
		global_work_size[1] = RoundToMaxWorkGroupSizeSqrtMultiple(*global_work_size);
		error = clEnqueueNDRangeKernel(queue, kernels[kernelName], 2, global_work_offset, global_work_size, localWorkGroupSizeSqrt, num_events_in_wait_list, event_wait_list, event);
	}

	void Mocl::Enqueue3DRangeKernel(std::string kernelName, const size_t* global_work_offset, size_t* global_work_size, cl_uint num_events_in_wait_list, const cl_event* event_wait_list, cl_event* event)
	{
		global_work_size[0] = RoundToMaxWorkGroupSizeCbrtMultiple(*global_work_size);
		global_work_size[1] = RoundToMaxWorkGroupSizeCbrtMultiple(*global_work_size);
		global_work_size[2] = RoundToMaxWorkGroupSizeCbrtMultiple(*global_work_size);
		error = clEnqueueNDRangeKernel(queue, kernels[kernelName], 3, global_work_offset, global_work_size, localWorkGroupSizeCbrt, num_events_in_wait_list, event_wait_list, event);
	}

	void Mocl::WaitForEvents(cl_uint numEvents, cl_event* eventList)
	{
		error = clWaitForEvents(numEvents, eventList);
	}

	void Mocl::WaitForEvents(cl_event* event)
	{
		error = clWaitForEvents(1, event);
	}

}