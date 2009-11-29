#include "stdafx.h"

#include <crtdbg.h>

#include <rgde/core/types.h>

#include <iostream>

// Macros for setting or clearing bits in the CRT debug flag 
#ifdef _DEBUG
#define  SET_CRT_DEBUG_FIELD(a)   _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

/* ALLOCATION HOOK FUNCTION
-------------------------
An allocation hook function can have many, many different
uses. This one simply logs each allocation operation in a file.
*/
int __cdecl MyAllocHook(
						int      nAllocType,
						void   * pvData,
						size_t   nSize,
						int      nBlockUse,
						long     lRequest,
						const unsigned char * szFileName,
						int      nLine
						)
{
	char *operation[] = { "", "allocating", "re-allocating", "freeing" };
	char *blockType[] = { "Free", "Normal", "CRT", "Ignore", "Client" };

	if ( nBlockUse == _CRT_BLOCK )   // Ignore internal C runtime library allocations
		return( 1/*TRUE*/ );

	_ASSERT( ( nAllocType > 0 ) && ( nAllocType < 4 ) );
	_ASSERT( ( nBlockUse >= 0 ) && ( nBlockUse < 5 ) );

	//std::cout << "Memory operation in " << szFileName << ", line " << nLine << ": "
	//	<< operation[nAllocType] << " a " << nSize << "-byte '" << blockType[nBlockUse]
	//<< "' block (#" << lRequest << std::endl;

	//std::cout << "Memory operation : "
	//	<< operation[nAllocType] << " a " << nSize << "-byte '" << blockType[nBlockUse]
	//<< "' block (#" << lRequest << std::endl;

	//fprintf( logFile, 
	//	"Memory operation in %s, line %d: %s a %d-byte '%s' block (#%ld)\n",
	//	szFileName, nLine, operation[nAllocType], nSize, 
	//	blockType[nBlockUse], lRequest );
	//if ( pvData != NULL )
	//	fprintf( logFile, " at %p", pvData );

	return( 1/*TRUE*/ );         // Allow the memory operation to proceed
}

#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)

#endif                     // End of #ifdef _DEBUG


namespace
{
	struct mem_alloc_handler;
	static mem_alloc_handler* mem_alloc_helper_instance = NULL;

	static struct mem_alloc_handler
	{
		mem_alloc_handler()
		{
			#ifdef _DEBUG
			// Send all reports to STDOUT, since this example is a console app
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
			_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
			_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
			_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
			_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
			_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
			SET_CRT_DEBUG_FIELD( _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF );
			// Install the hook functions
			_CrtSetAllocHook( MyAllocHook );
			mem_alloc_helper_instance = this;
			#endif                     // End of #ifdef _DEBUG
			
		}
		~mem_alloc_handler()
		{
			mem_alloc_helper_instance = NULL;
		}

	} static_instance____;
}


namespace rgde
{
	namespace core
	{
		namespace utils
		{
			int64 get_allocated_mem()
			{
				if (NULL != mem_alloc_helper_instance)
				{
					return 0;
				}
				else
				{
					return -1;
				}

			}

		}
	}
}