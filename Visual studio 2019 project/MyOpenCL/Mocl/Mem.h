#pragma once
#include <CL/cl.h>
namespace Mocl
{
	class Mem
	{
	private:
		cl_mem& _mem;
	public:
		Mem(cl_mem& mem);
		explicit operator cl_mem& ();
		explicit operator cl_mem* ();
		explicit operator const cl_mem& ();
		explicit operator const cl_mem* ();

	};
}



