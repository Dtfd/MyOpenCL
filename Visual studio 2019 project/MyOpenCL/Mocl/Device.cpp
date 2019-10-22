#include "Device.h"

namespace Mocl
{
	Device::Device(cl_device_id& deviceId) :_deviceId(deviceId) {}

	Device::operator cl_device_id& ()
	{
		return _deviceId;
	}
	Device::operator cl_device_id* ()
	{
		return &_deviceId;
	}
	Device::operator const cl_device_id& ()
	{
		return _deviceId;
	}
	Device::operator const cl_device_id* ()
	{
		return &_deviceId;
	}

	cl_int Device::CreateSubDevices(const cl_device_partition_property* properties, cl_uint numDevices, cl_device_id* outDevices, cl_uint* numDevicesRet)
	{
		return clCreateSubDevices(_deviceId,properties,numDevices,outDevices,numDevicesRet);
	}
	Device* Device::CreateSubDevices(const cl_device_partition_property* properties, cl_uint numDevices, cl_uint* numDevicesRet, cl_int* errorRet)
	{
		cl_device_id* clDevices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		*errorRet = clCreateSubDevices(_deviceId,properties,numDevices,clDevices,numDevicesRet);
		Device* outDevices = (Device*)malloc(*numDevicesRet * sizeof(Device));
		for (size_t i = 0; i < *numDevicesRet; outDevices[i] = Device(clDevices[i]), i++);
		free(clDevices);
		return outDevices;
	}
	std::vector<Device> Device::CreateSubDevices(const cl_device_partition_property* properties, cl_uint numDevices, cl_int* errorRet)
	{
		std::vector<Device> outDevices; 
		cl_device_id* clDevices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		cl_uint numDevicesRet;
		*errorRet = clCreateSubDevices(_deviceId, properties, numDevices, clDevices, &numDevicesRet);
		for (size_t i = 0; i < numDevicesRet; outDevices.push_back(Device(clDevices[i])), i++);
		free(clDevices);
		return outDevices;
	}

	cl_int Device::Release()
	{
		return clReleaseDevice(_deviceId);
	}
	cl_int Device::Retain()
	{
		return clRetainDevice(_deviceId);
	}

	cl_int Device::GetDeviceInfo(cl_device_info paramName, size_t paramValueSize, void* paramValue, size_t* paramValueSizeRet)
	{
		return clGetDeviceInfo(_deviceId,paramName,paramValueSize,paramValue,paramValueSizeRet);
	}

	DeviceInfo Device::GetInfo()
	{
		return DeviceInfo(_deviceId);
	}


}
