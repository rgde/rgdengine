#pragma once
#pragma warning(disable:4786) // truncation to '255' characters

#include <rgde/base/exceptions.h>

#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include <windows.h>

/*-----------------------------------------------------------------------------
	base_exception
-----------------------------------------------------------------------------*/
class	base_exception
{
	std::vector<std::string> stack;

public:
	base_exception& operator<< (const std::string& msg) { stack.push_back(msg); return *this; }

	virtual	std::string	stack_string();
	virtual	void	non_critical();		//called only in first catch if continuable()
	virtual	void	last_catch();		//called in last catch if !continuable()

	
	virtual	std::string	reason(){ return "base_exception";};
	virtual	bool continuable(){ return false;};

	// prototype clone auto generated in derived_exception<> helper.
	virtual	base_exception*	clone(){ return new base_exception(*this); };	
};

/*-----------------------------------------------------------------------------
	Helper template : derived_exception<>
-----------------------------------------------------------------------------*/
template <class T>
class	derived_exception : public base_exception{
public:
	virtual	base_exception*	clone(){ return new T(*((T*)this)); };
};

/*-----------------------------------------------------------------------------
	msg_exception
-----------------------------------------------------------------------------*/
class	msg_exception: public derived_exception<msg_exception>{
	std::string message;
public:
	msg_exception(const std::string msg):message(msg){}
	std::string reason(){ return message; }	
};

/*-----------------------------------------------------------------------------
	system_exception
-----------------------------------------------------------------------------*/
class	system_exception: public derived_exception<msg_exception>{
	unsigned	code;
	EXCEPTION_POINTERS	info;
public:
	system_exception(unsigned c,EXCEPTION_POINTERS& i):code(c),info(i){}
	bool continuable();
	std::string reason();
};
/*-----------------------------------------------------------------------------
	Hooks	
-----------------------------------------------------------------------------*/
typedef void (*translator_type)(unsigned, EXCEPTION_POINTERS *);
void my_translator(unsigned code, EXCEPTION_POINTERS * ep);
void term_func();
void on_exit( void );
typedef void (*unexpected_function)();
void my_unexpected();

/*-----------------------------------------------------------------------------
	guard
	unguard
	first_guard
	last_unguard
-----------------------------------------------------------------------------*/
#define guard(func)			{ const char* guard_scope_name = #func;\
								try{

#define unguard					}\
								catch (base_exception& ex ){\
									ex << guard_scope_name;\
									if(ex.continuable()) ex.non_critical();\
									else throw ex.clone();}\
								catch (std::exception& exp ){\
									base_exception* ex = new msg_exception(exp.what());\
									(*ex) << guard_scope_name;\
									throw ex;\
									}\
								catch (const char* ch ){\
									base_exception* ex = new msg_exception(ch);\
									(*ex) << guard_scope_name;\
									throw ex;\
								}\
								catch (...){\
									char	msg[512];\
									sprintf(msg, "Unhandled exception in %s.\n(last modified: %s)\n",__FILE__,__TIMESTAMP__);\
									base_exception* ex = new msg_exception(msg);\
									(*ex) << guard_scope_name;\
									throw ex;\
								}\
							}

/*-----------------------------------------------------------------------------
	First guard
-----------------------------------------------------------------------------*/
#define	first_guard(func)	{\
							translator_type old_tr = _set_se_translator(my_translator);\
							set_terminate( term_func );\
							unexpected_function old_un = set_unexpected( my_unexpected );\
							atexit(on_exit);\
							guard(func)

/*-----------------------------------------------------------------------------
	Last unguard
-----------------------------------------------------------------------------*/
#define last_unguard		}\
								catch (base_exception& ex ){\
									msg_exception bex(ex.reason());\
									bex << guard_scope_name;\
									bex.last_catch();\
								}\
								catch (base_exception* ex ){\
									(*ex) << guard_scope_name;\
									ex->last_catch();\
									delete ex;\
								}\
								catch (const char* ch ){\
									msg_exception msg(ch);\
									msg << guard_scope_name;\
									msg.last_catch();\
								}\
								catch (...){\
									char	msg[256];\
									sprintf(msg, "Unhandled exception in %s.\n(last modified: %s)\n",__FILE__,__TIMESTAMP__);\
									msg_exception ex(msg);\
									ex << guard_scope_name;\
									ex.last_catch();\
								}\
							}\
							_set_se_translator(old_tr);\
							set_unexpected( old_un );\
							}

#define V(x) { unsigned hr = x; if( 0 != hr ) { throw base::base_exception( #x, hr, __FILE__, __LINE__); } }

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=0; } }
#endif

