#include "Platform.h"
#include <stdlib.h>
namespace Mocl
{
	/*Constructorz*/
	Platform::Platform(cl_platform_id& platformId) : _platformId(platformId) {}

	/*easy usage in C functions */
	Platform::operator cl_platform_id& ()
	{
		return _platformId;
	}
	Platform::operator cl_platform_id* ()
	{
		return &_platformId;
	}
	Platform::operator const cl_platform_id& ()
	{
		return _platformId;
	}
	Platform::operator const cl_platform_id* ()
	{
		return &_platformId;
	}
	
	/*Get platform IDs*/
	cl_int Platform::GetIDs(cl_uint numEntries, cl_platform_id* platforms, cl_uint *numPlatforms)
	{
		return clGetPlatformIDs(numEntries, platforms, numPlatforms);
	}
	Platform* Platform::GetIDs(cl_uint numEntries, cl_uint* numPlatforms, cl_int *retError)
	{
		Platform* result = nullptr;
		cl_platform_id* clPlatforms = (cl_platform_id*)malloc(numEntries * sizeof(cl_platform_id));
		*retError = clGetPlatformIDs(numEntries, clPlatforms, numPlatforms);
		result = (Platform*)malloc(*numPlatforms * sizeof(Platform));
		for (size_t i = 0; i < *numPlatforms; result[i] = Platform(clPlatforms[i]), i++);
		free(clPlatforms);
		return result;

	}
	
	std::vector<Platform> Platform::GetIDs(cl_uint numEntries,cl_int* errorRet)
	{
		std::vector<Platform> result;
		cl_platform_id* clPlatforms = (cl_platform_id*)malloc(numEntries * sizeof(cl_platform_id));
		cl_uint numPlatforms;
		*errorRet = clGetPlatformIDs(numEntries, clPlatforms, &numPlatforms);
		for (size_t i = 0; i < numPlatforms; result.push_back(Platform(clPlatforms[i])), i++);
		free(clPlatforms);
		return result;
	}

	cl_int Platform::GetDefaultID(cl_platform_id* platform)
	{
		return clGetPlatformIDs(1,platform,NULL);
	}

	Platform Platform::GetDefaultID(cl_int* errorRet)
	{
		cl_platform_id clPlatform;
		*errorRet = clGetPlatformIDs(1, &clPlatform, NULL);
		return Platform(clPlatform);
	}
	
	/*Get device IDs*/
	cl_int Platform::GetDeviceIDs(cl_device_type deviceType, cl_uint numEntries, cl_device_id* devices, cl_uint* numDevices)
	{
		cl_int retError = CL_SUCCESS;
		retError = clGetDeviceIDs(_platformId, deviceType, numEntries, devices, numDevices);
		return retError;
	}
	Device* Platform::GetDeviceIDs(cl_device_type deviceType, cl_uint numEntries, cl_uint* numDevices, cl_int* errorRet)
	{
		Device* result = nullptr;
		cl_device_id* clDevices = (cl_device_id*)malloc(numEntries * sizeof(cl_device_id));
		*errorRet = clGetDeviceIDs(_platformId, deviceType, numEntries, clDevices, numDevices);
		result = (Device*)malloc(*numDevices * sizeof(Device));
		for (size_t i = 0; i < *numDevices; result[i] = Device(clDevices[i]), i++);
		free(clDevices);
		return result;
	}

	std::vector<Device> Platform::GetDeviceIDs(cl_device_type deviceType, cl_uint numEntries, cl_int* errorRet)
	{
		std::vector<Device> outDevices;
		cl_device_id* clDevices = (cl_device_id*)malloc(numEntries * sizeof(cl_device_id));
		cl_uint numDevices;
		*errorRet = clGetDeviceIDs(_platformId, deviceType, numEntries, clDevices, &numDevices);
		for (size_t i = 0; i < numDevices; outDevices.push_back(Device(clDevices[i])), i++);
		free(clDevices);
		return outDevices;
	}

	cl_int Platform::GetDefaultDeviceID(cl_device_type deviceType, cl_device_id* device)
	{
		return clGetDeviceIDs(_platformId,deviceType,1,device,NULL);
	}

	Device Platform::GetDefaultDeviceID(cl_device_type deviceType, cl_int* errorRet)
	{
		cl_device_id clDevice;
		*errorRet = clGetDeviceIDs(_platformId, deviceType, 1, &clDevice, NULL);
		return Device(clDevice);
	}

	
	/*Get info*/
	cl_int Platform::GetPlatformInfo(cl_platform_info paramName, size_t paramValueSize, void* paramValue, size_t* paramValueSizeRet)
	{
		return clGetPlatformInfo(_platformId, paramName, paramValueSize, paramValue, paramValueSizeRet);
	}

	PlatformInfo Platform::GetInfo()
	{
		return PlatformInfo(_platformId);
	}
	

}
