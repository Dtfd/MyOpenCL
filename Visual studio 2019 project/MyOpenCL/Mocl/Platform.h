#pragma once
#include <CL/cl.h>
#include "PlatformInfo.h"
#include "Device.h"
#include <string>
#include <vector>
namespace Mocl
{
	class Platform
	{
	private:
		cl_platform_id _platformId;
	public:
		Platform(cl_platform_id& platformId);

		/*easy usage in C functions */
		explicit operator cl_platform_id&();
		explicit operator cl_platform_id*();
		explicit operator const cl_platform_id&();
		explicit operator const cl_platform_id*();

		/*Get platform IDs*/
		static cl_int GetIDs(cl_uint numEntries,
							 cl_platform_id* platforms,
							 cl_uint* numPlatforms);
		static Platform* GetIDs(cl_uint numEntries,
								cl_uint* numPlatforms,
								cl_int* errorRet);
		static std::vector<Platform> GetIDs(cl_uint numEntries, cl_int* errorRet);

		static cl_int GetDefaultID(cl_platform_id* platform);
		static Platform GetDefaultID(cl_int* errorRet);
		/*Get device IDs*/
		cl_int GetDeviceIDs(cl_device_type deviceType,
							cl_uint numEntries,
							cl_device_id* devices,
							cl_uint* numDevices);
		Device* GetDeviceIDs(cl_device_type deviceType,
							 cl_uint numEntries,
							 cl_uint* numDevices,
							 cl_int* errorRet);
		std::vector<Device> GetDeviceIDs(cl_device_type deviceType,
										 cl_uint numEntries,
										 cl_int* errorRet);
		cl_int GetDefaultDeviceID(cl_device_type deviceType,cl_device_id* deviceRet);
		Device GetDefaultDeviceID(cl_device_type deviceType, cl_int* errorRet);


		/*Get info*/
		cl_int GetPlatformInfo(cl_platform_info paramName,
					   size_t paramValueSize,
					   void* paramValue,
					   size_t* paramValueSizeRet);
		PlatformInfo GetInfo();


	};
}


