#include "DeviceInfo.h"

namespace Mocl
{
	DeviceInfo::DeviceInfo(cl_device_id& deviceId) :_deviceId(deviceId){}
	std::string DeviceInfo::Name(cl_int* errorRet)
	{
		std::string result;
		char tmp[64];
		size_t size;
		*errorRet = clGetDeviceInfo(_deviceId, CL_DEVICE_NAME, 64 * sizeof(char), tmp, &size);
		result = std::string(tmp, size);
		//result.shrink_to_fit();
		return result;
	}
}
