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
		Context(cl_context context);
		Context (const cl_context_properties* properties,
			  cl_uint numDevices,
			  const cl_device_id* devices,
			  void (CL_CALLBACK* pfnNotify) (const char* errinfo,const void* privateInfo, size_t cb, void* userData),
			  void* user_Data,
			  cl_int* errcodeRet);

		Context (const cl_context_properties* properties,
			  cl_uint numDevices,
			  const Device* devices,
			  void (CL_CALLBACK* pfnNotify) (const char* errinfo,const void* privateInfo, size_t cb, void* userData),
			  void* userData,
			  cl_int* errcodeRet);
								 
		Context (const cl_context_properties* properties,
			  std::vector<Device>* devices,
			  void (CL_CALLBACK* pfnNotify) (const char* errinfo,const void* privateInfo, size_t cb, void* userData),
			  void* userData,
			  cl_int* errcodeRet);

		Context (const cl_context_properties* properties,
				  cl_device_type  device_type,
				  void (CL_CALLBACK* pfn_notify) (const char* errinfo,const void* private_info, size_t cb,void* user_data),
				  void* user_data,
				  cl_int* errcode_ret);

		static Context CreateContext(const cl_context_properties* properties,
									 cl_uint numDevices,
									 const cl_device_id* devices,
									 void (CL_CALLBACK* pfnNotify) (const char* errinfo,const void* privateInfo, size_t cb, void* userData),
									 void* user_Data,
									 cl_int* errcodeRet);

		static Context CreateContext(const cl_context_properties* properties,
									 cl_uint numDevices,
									 const Device* devices,
									 void (CL_CALLBACK* pfnNotify) (const char* errinfo,const void* privateInfo, size_t cb, void* userData),
									 void* userData,
									 cl_int* errcodeRet);
									 
		static Context CreateContext(const cl_context_properties* properties,
									 std::vector<Device>* devices,
									 void (CL_CALLBACK* pfnNotify) (const char* errinfo,const void* privateInfo, size_t cb, void* userData),
									 void* userData,
									 cl_int* errcodeRet);

		static Context CreateContextFromType(const cl_context_properties* properties,
											 cl_device_type  device_type,
											 void (CL_CALLBACK* pfn_notify) (const char* errinfo,const void* private_info, size_t cb,void* user_data),
											 void* user_data,
											 cl_int* errcode_ret);
		explicit operator cl_context& ();
		explicit operator cl_context* ();
		explicit operator const cl_context& ();
		explicit operator const cl_context* ();

		cl_int Release();
		cl_int Retain();

	};
}


