#include "PlatformInfo.h"
#define CHAR_BUFFOR_SIZE_S 32
#define CHAR_BUFFOR_SIZE_M 64
#define CHAR_BUFFOR_SIZE_L 512
#define CHAR_BUFFOR_SIZE_XL 1024
#define CHAR_BUFFOR_SIZE_XXL 2048
#include <sstream>
namespace Mocl
{
	PlatformInfo::PlatformInfo(cl_platform_id& platformId) :_platformId(platformId) {}
	std::string PlatformInfo::Profile(cl_int* errorRet)
	{
		std::string result;
		char tmp[CHAR_BUFFOR_SIZE_S];
		size_t size;
		*errorRet = clGetPlatformInfo(_platformId, CL_PLATFORM_PROFILE, CHAR_BUFFOR_SIZE_S * sizeof(char), tmp, &size);
		result = std::string(tmp, size);
		result.shrink_to_fit();
		return result;
	}
	std::string PlatformInfo::Version(cl_int* errorRet)
	{
		std::string result;
		char tmp[CHAR_BUFFOR_SIZE_M];
		size_t size;
		*errorRet = clGetPlatformInfo(_platformId, CL_PLATFORM_VERSION, CHAR_BUFFOR_SIZE_M * sizeof(char), tmp, &size);
		result = std::string(tmp, size);
		result.shrink_to_fit();
		return result;
	}
	std::string PlatformInfo::Name(cl_int* errorRet)
	{
		std::string result;
		char tmp[CHAR_BUFFOR_SIZE_XL];
		size_t size;
		*errorRet = clGetPlatformInfo(_platformId, CL_PLATFORM_NAME, CHAR_BUFFOR_SIZE_XL * sizeof(char), tmp, &size);
		result = std::string(tmp, size);
		result.shrink_to_fit();
		return result;
	}
	std::string PlatformInfo::Vendor(cl_int* errorRet)
	{
		std::string result;
		char tmp[CHAR_BUFFOR_SIZE_M];
		size_t size;
		*errorRet = clGetPlatformInfo(_platformId, CL_PLATFORM_VENDOR, CHAR_BUFFOR_SIZE_M * sizeof(char), tmp, &size);
		result = std::string(tmp, size);
		result.shrink_to_fit();
		return result;
	}
	std::string PlatformInfo::Extensions(cl_int* errorRet)
	{
		std::string result;
		char tmp[CHAR_BUFFOR_SIZE_XL];
		size_t size;
		*errorRet = clGetPlatformInfo(_platformId, CL_PLATFORM_EXTENSIONS, CHAR_BUFFOR_SIZE_XL * sizeof(char), tmp, &size);
		result = std::string(tmp, size);
		result.shrink_to_fit();
		return result;
	}
	std::vector<std::string> PlatformInfo::ExtensionsVect(cl_int* errorRet)
	{
		std::vector<std::string> result;
		char tmp[CHAR_BUFFOR_SIZE_XL];
		size_t size;
		*errorRet = clGetPlatformInfo(_platformId, CL_PLATFORM_EXTENSIONS, CHAR_BUFFOR_SIZE_XL * sizeof(char), tmp, &size);
		tmp[CHAR_BUFFOR_SIZE_XL - 1] = '\0';
		std::istringstream f(tmp);
		std::string s;
		while (getline(f, s, ' ')) {
			result.push_back(s);
		}
		return result;
	}
	cl_ulong PlatformInfo::HostTimerResolution(cl_int* errorRet)
	{
		cl_ulong result;
		size_t size;
		*errorRet = clGetPlatformInfo(_platformId, CL_PLATFORM_PROFILE, sizeof(char), &result, &size);
		return result;
	}
}
