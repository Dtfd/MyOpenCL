#include "CommandQueue.h"

namespace Mocl
{
	CommandQueue::CommandQueue(cl_command_queue& commandQueue): _commandQueue(commandQueue){}
	CommandQueue::operator cl_command_queue& ()
	{
		return _commandQueue;
	}
	CommandQueue::operator cl_command_queue* ()
	{
		return &_commandQueue;
	}
	CommandQueue::operator const cl_command_queue& ()
	{
		return _commandQueue;
	}
	CommandQueue::operator const cl_command_queue* ()
	{
		return &_commandQueue;
	}
}