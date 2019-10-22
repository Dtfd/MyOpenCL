#pragma once
#include <CL/cl.h>
#include <vector>
#include "Device.h"
namespace Mocl
{
	class Context
	{
	private:
		cl_context _context;
	public:
		Context(cl_context& context);
		Context CreateContext(const cl_context_properties* properties,
							  cl_uint numDevices,
							  const cl_device_id* devices,
							  void (CL_CALLBACK* pfnNotify) (const char* errinfo, void* privateInfo, size_t cb, void* userData),
							  void* user_Data,
							  cl_int* errcodeRet);

		Context CreateContext(const cl_context_properties* properties,
							  cl_uint numDevices,
							  const Device* devices,
							  void (CL_CALLBACK* pfnNotify) (const char* errinfo, void* privateInfo, size_t cb, void* userData),
							  void* userData,
							  cl_int* errcodeRet);

		Context CreateContext(const cl_context_properties* properties,
							  std::vector<Device>* devices,
							  void (CL_CALLBACK* pfnNotify) (const char* errinfo, void* privateInfo, size_t cb, void* userData),
							  void* userData,
							  cl_int* errcodeRet);

		explicit operator cl_context& ();
		explicit operator cl_context* ();
		explicit operator const cl_context& ();
		explicit operator const cl_context* ();

	};
}


