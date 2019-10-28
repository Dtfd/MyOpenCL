#include <iostream>
#include <Windows.h>
#define CW(msg) std::cout << msg;
#define CWL(msg) std::cout << msg << std::endl;
#include "Mocl/Platform.h"//OK
#include "Mocl/PlatformInfo.h"//OK
#include "Mocl/Device.h"//OK
#include "Mocl/DeviceInfo.h"//Not implemented!!!!!!!!!
#include "Mocl/Context.h"//only usage as container
#include "Mocl/CommandQueue.h"//only usage as container
#include "Mocl/Mem.h"//only usage as container

void main000() 
{/*
	cl_int error = 0;
	cl_uint numPlatf = 0;
	cl_uint numDev = 0;
	Mocl::Platform* platf = Mocl::Platform::GetIDs(10, &numPlatf, &error);
	Mocl::Device* dev = platf[0].GetDeviceIDs(CL_DEVICE_TYPE_ALL, 10, &numDev, &error);
	Mocl::PlatformInfo pInf = platf[0].GetInfo();
	Mocl::Device device(dev[0]);
	std::cout << pInf.GetInfoProfile(&error) << std::endl;
	std::cout << pInf.GetInfoVersion(&error) << std::endl;
	std::cout << pInf.GetInfoName(&error) << std::endl;
	std::cout << pInf.GetInfoVendor(&error) << std::endl;
	std::cout << pInf.GetInfoExtensions(&error) << std::endl;
	std::cout << pInf.GetInfoExtensionsVect(&error)[0] << std::endl;
	std::cout << pInf.GetInfoHostTimerResolution(&error) << std::endl;
	free(platf);
	free(dev);*/
}

void main001() 
{
	cl_int error = 0;
	std::vector<Mocl::Platform> allPlatforms = Mocl::Platform::GetIDs(5, &error);
	std::cout << "numPlatforms: " << allPlatforms.size() << std::endl;
	for (size_t i = 0; i < allPlatforms.size(); i++)
	{
		std::vector<Mocl::Device> devices = allPlatforms[i].GetDeviceIDs(CL_DEVICE_TYPE_ALL, 10, &error);
		std::cout << allPlatforms[i].GetInfo().Name(&error) << " ("<< devices.size() << ")" << std::endl;
		for (size_t j = 0; j < devices.size(); j++)
		{
			std::cout << "\t" << devices[j].GetInfo().Name(&error) << std::endl;
		}
	}
}

void main002()
{
	cl_int error = 0;
	Mocl::Platform platform = Mocl::Platform::GetDefaultID(&error);
	Mocl::Device device = platform.GetDefaultDeviceID(CL_DEVICE_TYPE_GPU, &error);
	std::cout << "Platform: " << platform.GetInfo().Name(&error) << "\n\tDevice: " << device.GetInfo().Name(&error);
}

void main003()
{
	cl_int error = 0;
	Mocl::Platform platform = Mocl::Platform::GetDefaultID(&error);
	CWL("Mocl::Platform::GetDefaultID " << error);
	Mocl::Device device = platform.GetDefaultDeviceID(CL_DEVICE_TYPE_GPU, &error);
	CWL("platform.GetDefaultDeviceID " << error);
	CWL("Platform: " << platform.GetInfo().Name(&error) << "platform.GetInfo().Name() " << error << "\n\tDevice: " << device.GetInfo().Name(&error) << "device.GetInfo() " << error);
	Mocl::Context ctxA = Mocl::Context::CreateContext(NULL, 1, &device, NULL, NULL, &error);
	CWL("Mocl::Context::CreateContext " << error);
	device.Release();
	ctxA.Release();
}

void main004()
{
	int i = 0;
	while(true)
	{
		cl_int error = 0;
		Mocl::Platform platform = Mocl::Platform::GetDefaultID(&error);
		Mocl::Device device = platform.GetDefaultDeviceID(CL_DEVICE_TYPE_GPU, &error);
		Mocl::Context ctxA(NULL, 1, &device, NULL, NULL, &error);
		device.Release();
		ctxA.Release();
		if(i%1000 == 0)Sleep(2500);
		i++;
	}
}
void main005()
{
	
}
int main()
{
	main004();
	std::cin.ignore();
}
