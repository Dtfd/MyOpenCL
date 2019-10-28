#include "Context.h"

namespace Mocl
{
	Context::Context(cl_context& context) :_context(context) {}
	Context::Context(cl_context context) :_context(context) {}

	Context::Context(const cl_context_properties* properties, cl_uint numDevices, const cl_device_id* devices, void (CL_CALLBACK* pfnNotify) (const char* errinfo, const void* privateInfo, size_t cb, void* userData), void* user_Data, cl_int* errcodeRet)
	:_context(clCreateContext(properties, numDevices, devices, pfnNotify, user_Data, errcodeRet)){}
	

	Context::Context(const cl_context_properties* properties, cl_uint numDevices, const Device* devices, void (CL_CALLBACK* pfnNotify) (const char* errinfo, const void* privateInfo, size_t cb, void* userData), void* userData, cl_int* errcodeRet)
	{
		cl_device_id* clDevices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		for (size_t i = 0; i < numDevices; i++)
		{
			clDevices[i] = (cl_device_id&)devices[i];
		}
		_context = clCreateContext(properties, numDevices, clDevices, pfnNotify, userData, errcodeRet);
		free(clDevices);
	}

	Context::Context(const cl_context_properties* properties, std::vector<Device>* devices, void (CL_CALLBACK* pfnNotify) (const char* errinfo, const void* privateInfo, size_t cb, void* userData), void* userData, cl_int* errcodeRet)
	{
		cl_device_id* clDevices = (cl_device_id*)malloc(devices->size() * sizeof(cl_device_id));
		for (size_t i = 0; i < devices->size(); i++)
		{
			clDevices[i] = (cl_device_id&)devices[i];
		}
		_context = clCreateContext(properties, devices->size(), clDevices, pfnNotify, userData, errcodeRet);
		free(clDevices);
	}

	Context::Context(const cl_context_properties* properties, cl_device_type device_type, void (CL_CALLBACK* pfnNotify) (const char* errinfo, const void* privateInfo, size_t cb, void* userData), void* user_data, cl_int* errcode_ret)
	:_context(clCreateContextFromType(properties, device_type, pfnNotify, user_data, errcode_ret)){}
	
	Context Context::CreateContext(const cl_context_properties* properties, cl_uint numDevices, const cl_device_id* devices, void (CL_CALLBACK* pfnNotify) (const char* errinfo,const void* privateInfo, size_t cb, void* userData), void* userData, cl_int* errorRet)
	{
		return clCreateContext(properties,numDevices,devices,pfnNotify,userData,errorRet);
	}
	Context Context::CreateContext(const cl_context_properties* properties, cl_uint numDevices, const Device* devices, void (CL_CALLBACK* pfnNotify) (const char* errinfo, const void* privateInfo, size_t cb, void* userData), void* userData, cl_int* errorRet)
	{
		cl_device_id* clDevices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		for (size_t i = 0; i < numDevices; i++)
		{
			clDevices[i] = (cl_device_id&)devices[i];
		}
		Context tmp(clCreateContext(properties, numDevices, clDevices, pfnNotify, userData, errorRet));
		free(clDevices);
		return tmp;
	}
	Context Context::CreateContext(const cl_context_properties* properties, std::vector<Device>* devices, void (CL_CALLBACK* pfnNotify) (const char* errinfo, const void* privateInfo, size_t cb, void* userData), void* userData, cl_int* errorRet)
	{
		cl_device_id* clDevices = (cl_device_id*)malloc(devices->size() * sizeof(cl_device_id));
		for (size_t i = 0; i < devices->size(); i++)
		{
			clDevices[i] = (cl_device_id&)devices[i];
		}
		Context tmp(clCreateContext(properties, devices->size(), clDevices, pfnNotify, userData, errorRet));
		free(clDevices);
		return tmp;
	}
	
	Context Context::CreateContextFromType(const cl_context_properties* properties, cl_device_type device_type, void (CL_CALLBACK* pfn_notify) (const char* errinfo, const void* private_info, size_t cb, void* user_data), void* user_data, cl_int* errcode_ret)
	{
		return Context(clCreateContextFromType(properties,device_type,pfn_notify,user_data,errcode_ret));
	}
	
	Context::operator cl_context& ()
	{
		return _context;
	}
	Context::operator cl_context* ()
	{
		return &_context;
	}
	Context::operator const cl_context& ()
	{
		return _context;
	}
	Context::operator const cl_context* ()
	{
		return &_context;
	}

	cl_int Context::Release()
	{
		return clReleaseContext(_context);
	}
	cl_int Context::Retain()
	{
		return clRetainContext(_context);
	}
}
