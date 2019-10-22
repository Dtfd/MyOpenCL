#include "Context.h"

namespace Mocl
{
	Context::Context(cl_context& context) :_context(context) {}
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
}
