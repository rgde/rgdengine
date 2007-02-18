/*=============================================================================
	Exception.cpp:	(created 2001/08/11 	12:18)
	
	Copyright (c) 2001.  All Rights Reserved.
  
    Authors:
	Artyom V. Borodkin         (http://www.deep-dream.com)
	
=============================================================================*/
#include "precompiled.h"
#include "exception.h"

/*-----------------------------------------------------------------------------
	base_exception
-----------------------------------------------------------------------------*/
std::string	base_exception::stack_string()
{
	std::string out;
	int stack_size = (int)stack.size();
	for(int i = stack_size-1; i >=0 ;i--){
		out+= stack[i];
		if(i)out+= std::string(" -> ");
	}
	return out;
}
void	base_exception::non_critical()
{
	last_catch();
}

void base_exception::last_catch()
{
		std::string verdict;
		if(continuable())	
			verdict = "Verdict: Continue";
		else				
			verdict = "Verdict: Terminate.";

		base::lerr  << "******************** exception ************************";
		base::lerr	<< "Call stack: " << stack_string();
		base::lerr	<< "Reason: " << reason();
		base::lerr	<< verdict;
		base::lerr	<< "*******************************************************";

		//cout << info.c_str();
		//MessageBox(NULL,info.c_str(),"Exception",MB_OK | MB_ICONERROR);
}

/*-----------------------------------------------------------------------------
	system_exception
-----------------------------------------------------------------------------*/
bool system_exception::continuable()
{
	return info.ExceptionRecord->ExceptionFlags == 0;
}

std::string system_exception::reason()
{
	std::string out;
	
	switch (code){
	case EXCEPTION_ACCESS_VIOLATION          : out += "ACCESS_VIOLATION:The thread tried to read from or write to a virtual address for which it does not have the appropriate access."; break;
	case EXCEPTION_DATATYPE_MISALIGNMENT     : out += "DATATYPE_MISALIGNMENT:The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on."; break;
	case EXCEPTION_BREAKPOINT                : out += "BREAKPOINT:A breakpoint was encountered."; break;
	case EXCEPTION_SINGLE_STEP               : out += "SINGLE_STEP:A trace trap or other single-instruction mechanism signaled that one instruction has been executed."; break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED     : out += "ARRAY_BOUNDS_EXCEEDED:The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking."; break;
	case EXCEPTION_FLT_DENORMAL_OPERAND      : out += "FLOAT_DENORMAL_OPERAND:One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value."; break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO        : out += "FLOAT_DIVIDE_BY_ZERO:The thread tried to divide a floating-point value by a floating-point divisor of zero."; break;
	case EXCEPTION_FLT_INEXACT_RESULT        : out += "FLOAT_INEXACT_RESULT:The result of a floating-point operation cannot be represented exactly as a decimal fraction."; break;
	case EXCEPTION_FLT_INVALID_OPERATION     : out += "FLOAT_INVALID_OPERATION:This exception represents any floating-point exception not included in this list."; break;
	case EXCEPTION_FLT_OVERFLOW              : out += "FLOAT_OVERFLOW:The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type."; break;
	case EXCEPTION_FLT_STACK_CHECK           : out += "FLOAT_STACK_CHECK:The stack overflowed or underflowed as the result of a floating-point operation."; break;
	case EXCEPTION_FLT_UNDERFLOW             : out += "FLOAT_UNDERFLOW:The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type."; break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO        : out += "INTEGER_DIVIDE_BY_ZERO: The thread tried to divide an integer value by an integer divisor of zero."; break;
	case EXCEPTION_INT_OVERFLOW              : out += "INTEGER_OVERFLOW:The result of an integer operation caused a carry out of the most significant bit of the result."; break;
	case EXCEPTION_PRIV_INSTRUCTION          : out += "PRIVILEGED_INSTRUCTION:The thread tried to execute an instruction whose operation is not allowed in the current machine mode."; break;
	case EXCEPTION_IN_PAGE_ERROR             : out += "IN_PAGE_ERROR:The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network."; break;
	case EXCEPTION_ILLEGAL_INSTRUCTION       : out += "ILLEGAL_INSTRUCTION:The thread tried to execute an invalid instruction."; break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION  : out += "NONCONTINUABLE_EXCEPTION:The thread tried to continue execution after a noncontinuable exception occurred."; break;
	case EXCEPTION_STACK_OVERFLOW            : out += "STACK_OVERFLOW:The thread used up its stack."; break;
	case EXCEPTION_INVALID_DISPOSITION       : out += "INVALID_DISPOSITION:An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception."; break;
	case EXCEPTION_GUARD_PAGE                : out += "GUARD_PAGE_VIOLATION"; break;
	case EXCEPTION_INVALID_HANDLE            : out += "INVALID_HANDLE"; break;
	}

	//char	msg[256];
	//sprintf(msg, "\nAddress : %p : ",info.ExceptionRecord->ExceptionAddress);
	//out += msg;

	if(info.ExceptionRecord->ExceptionFlags == 0){
		out += "EXCEPTION_CONTINUABLE";
	}else{
		out += "EXCEPTION_NONCONTINUABLE";
	}

	return out;
}
/*-----------------------------------------------------------------------------
	hooks
-----------------------------------------------------------------------------*/

void my_translator(unsigned code, EXCEPTION_POINTERS * ep)
{	
	throw new system_exception(code,*ep);	
}
void term_func()
{
	base::lerr << "???????????????" << " term_func()";
	exit(-1);
}
void my_unexpected()
{
	base::lerr << "???????????????" << " my_unexpected()";
	exit(-1);
}
void on_exit( void )
{
	//base::lerr << __FILE__ << " [onExit]";
}