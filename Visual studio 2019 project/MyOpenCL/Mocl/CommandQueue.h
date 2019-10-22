#pragma once
#include <CL/cl.h>
namespace Mocl
{
	class CommandQueue
	{
	private:
		cl_command_queue& _commandQueue;
	public:
		CommandQueue(cl_command_queue& platformId);
		explicit operator cl_command_queue& ();
		explicit operator cl_command_queue* ();
		explicit operator const cl_command_queue& ();
		explicit operator const cl_command_queue* ();
	};
}


