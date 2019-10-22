#include <iostream>
#include "Mocl/Platform.h"//OK
#include "Mocl/PlatformInfo.h"//OK
#include "Mocl/Device.h"//OK
#include "Mocl/DeviceInfo.h"//Not implemented!!!!!!!!!
#include "Mocl/Context.h"//only usage as container
#include "Mocl/CommandQueue.h"//only usage as container
#include "Mocl/Mem.h"//only usage as container
void main002()
{
	cl_int error = 0;
	Mocl::Platform platform = Mocl::Platform::GetDefaultID(&error);
	Mocl::Device device = platform.GetDefaultDeviceID(CL_DEVICE_TYPE_GPU, &error);
	std::cout << "Platform: " << platform.GetInfo().Name(&error) << "\n\tDevice: " << device.GetInfo().Name(&error);
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

int main()
{
	main002();
	std::cin.ignore();
}
