#pragma once
#include <CL/cl.h>
#include <string>
#include <vector>

namespace Mocl
{
	class PlatformInfo
	{
	private:
		cl_platform_id& _platformId;
	public:
		PlatformInfo(cl_platform_id& platformId);
		std::string Profile(cl_int* errorRet);
		std::string Version(cl_int* errorRet);
		std::string Name(cl_int* errorRet);
		std::string Vendor(cl_int* errorRet);
		std::string Extensions(cl_int* errorRet);
		std::vector<std::string> ExtensionsVect(cl_int* errorRet);
		cl_ulong HostTimerResolution(cl_int* errorRet);
	};
}


