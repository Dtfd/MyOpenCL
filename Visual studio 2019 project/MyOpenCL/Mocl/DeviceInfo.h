#pragma once
#include <CL/cl.h>
#include <string>
#include <vector>
namespace Mocl
{
	class DeviceInfo
	{
	private:
		cl_device_id& _deviceId;
	public:
		DeviceInfo(cl_device_id& deviceId);
		cl_uint AddressBits(cl_int* errorRet);
		cl_bool Available(cl_int* errorRet);
		std::string BuiltInKernels(cl_int* errorRet);
		cl_bool CompilerAvailable(cl_int* errorRet);
		cl_device_fp_config DoubleFpConfig(cl_int* errorRet);
		cl_bool EndianLittle(cl_int* errorRet);
		cl_bool ErrorCorrectionSupport(cl_int* errorRet);
		cl_device_exec_capabilities ExecutionCapabilities(cl_int* errorRet);
		std::string Extensions(cl_int* errorRet);
		std::vector<std::string> ExtensionsVect(cl_int* errorRet);
		cl_ulong GlobalMemCacheSize(cl_int* errorRet);
		cl_device_mem_cache_type GlobalMemCacheType(cl_int* errorRet);
		cl_uint GlobalMemCachelineSize(cl_int* errorRet);
		cl_ulong GlobalMemSize(cl_int* errorRet);
		size_t GlobalVariablePreferedTotalSize(cl_int* errorRet);
		std::string IlVersion(cl_int* errorRet);
		size_t Image2DMaxHeight(cl_int* errorRet);
		size_t Image2DMaxWidth(cl_int* errorRet);
		size_t Image3DMaxHeight(cl_int* errorRet);
		size_t Image3DMaxWidth(cl_int* errorRet);
		size_t ImageBaseAddressAligment(cl_int* errorRet);
		size_t ImageMaxArraySize(cl_int* errorRet);
		size_t ImageMaxBufferSize(cl_int* errorRet);
		cl_uint ImagePitchAligment(cl_int* errorRet);
		cl_bool ImageSupport(cl_int* errorRet);
		cl_bool LinkerAvailable(cl_int* errorRet);
		cl_ulong LocalMemSize(cl_int* errorRet);
		cl_device_local_mem_type LocalMemType(cl_int* errorRet);
		cl_uint MaxClockFrequency(cl_int* errorRet);
		cl_uint MaxComputeUnits(cl_int* errorRet);
		cl_ulong MaxConstantArgs(cl_int* errorRet);
		cl_ulong MaxConstantBufferSize(cl_int* errorRet);
		cl_ulong MaxGlobalVariableSize(cl_int* errorRet);
		cl_ulong MaxMemAllocSize(cl_int* errorRet);
		cl_uint MaxNumSubGroups(cl_int* errorRet);
		cl_uint MaxOnDeviceEvents(cl_int* errorRet);
		cl_uint MaxOnDeviceQueues(cl_int* errorRet);
		size_t MaxParameterSize(cl_int* errorRet);
		cl_uint MaxPipeArgs(cl_int* errorRet);
		cl_uint MaxReadImageArgs(cl_int* errorRet);
		cl_uint MaxReadWriteImageArgs(cl_int* errorRet);
		cl_uint MaxSamplers(cl_int* errorRet);
		size_t MaxWorkGroupSize(cl_int* errorRet);
		cl_uint MaxWorkItemDimentions(cl_int* errorRet);
		size_t* MaxWorkItemSizes(cl_int* errorRet);
		cl_uint MaxWriteImageArgs(cl_int* errorRet);
		cl_uint MemBaseAddrAlignSize(cl_int* errorRet);
		std::string Name(cl_int* errorRet);
		cl_uint NativeVectorWidthChar(cl_int* errorRet);
		cl_uint NativeVectorWidthShort(cl_int* errorRet);
		cl_uint NativeVectorWidthInt(cl_int* errorRet);
		cl_uint NativeVectorWidthLong(cl_int* errorRet);
		cl_uint NativeVectorWidthFloat(cl_int* errorRet);
		cl_uint NativeVectorWidthDouble(cl_int* errorRet);
		cl_uint NativeVectorWidthHalf(cl_int* errorRet);
		std::string OpenCLVersion(cl_int* errorRet);
		cl_device_id ParentDevice(cl_int* errorRet);
		cl_device_partition_property* PartitionProperties(cl_int* errorRet);
		cl_device_affinity_domain PartitionAfinityDomain(cl_int* errorRet);
		cl_device_partition_property* DevicePartitionType(cl_int* errorRet);
		cl_platform_id Platform(cl_int* errorRet);
		cl_uint PreferredVectorWidthChar(cl_int* errorRet);
		cl_uint PreferredVectorWidthShort(cl_int* errorRet);
		cl_uint PreferredVectorWidthInt(cl_int* errorRet);
		cl_uint PreferredVectorWidthLong(cl_int* errorRet);
		cl_uint PreferredVectorWidthFloat(cl_int* errorRet);
		cl_uint PreferredVectorWidthDouble(cl_int* errorRet);
		cl_uint PreferredVectorWidthHalf(cl_int* errorRet);
		size_t PrintfBufferSize(cl_int* errorRet);
		cl_bool PreferredInteropUserSync(cl_int* errorRet);
		std::string Profile(cl_int* errorRet);
		cl_command_queue_properties QueueProperties(cl_int* errorRet);
		cl_uint ReferenceCount(cl_int* errorRet);
		cl_device_fp_config SingleFpConfig(cl_int* errorRet);
		cl_device_type Type(cl_int* errorRet);
		std::string Vendor(cl_int* errorRet);
		cl_uint VendorID(cl_int* errorRet);
		std::string DeviceVersion(cl_int* errorRet);
		std::string DriverVersion(cl_int* errorRet);
	};
}


