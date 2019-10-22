#include "Mem.h"

namespace Mocl
{
	Mem::Mem(cl_mem& mem) :_mem(mem) {}

	Mem::operator cl_mem& ()
	{
		return _mem;
	}

	Mem::operator cl_mem* ()
	{
		return &_mem;
	}

	Mem::operator const cl_mem& ()
	{
		return _mem;
	}

	Mem::operator const cl_mem* ()
	{
		return &_mem;
	}

}
