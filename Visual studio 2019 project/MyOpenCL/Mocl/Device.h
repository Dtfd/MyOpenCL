#pragma once
#include <CL/cl.h>
#include "DeviceInfo.h"
#include <string>
#include <vector>
namespace Mocl
{
	class Device
	{
	private:
		cl_device_id _deviceId;
	public:
		Device();
		Device(cl_device_id& deviceId);
		explicit operator cl_device_id& ();
		explicit operator cl_device_id* ();
		explicit operator const cl_device_id& ();
		explicit operator const cl_device_id* ();

		cl_int CreateSubDevices(const cl_device_partition_property* properties,
								cl_uint  numDevices,
								cl_device_id* outDevices,
								cl_uint* numDevicesRet);
		Device* CreateSubDevices(const cl_device_partition_property* properties,
								cl_uint  numDevices,
								cl_uint* numDevicesRet,
								cl_int* errorRet);
		std::vector<Device> CreateSubDevices(const cl_device_partition_property* properties,
											 cl_uint  numDevices,
											 cl_int* errorRet);
		cl_int Release();
		cl_int Retain();

		/*cl_context CreateClContext(const cl_context_properties* properties,
								   void (CL_CALLBACK* pfn_notify) (const char* errinfo, const void* private_info, size_t cb, void* user_data),
								   void* user_data,
								   cl_int* errcode_ret);
		Context CreateContext(const cl_context_properties* properties,
							  void (CL_CALLBACK* pfn_notify) (const char* errinfo, const void* private_info, size_t cb, void* user_data),
							  void* user_data,
							  cl_int* errcode_ret);*/
		
		cl_int GetDeviceInfo(cl_device_info  paramName,
							   size_t  paramValueSize,
							   void* paramValue,
							   size_t* paramValueSizeRet);
		DeviceInfo GetInfo();
		
	};
}


