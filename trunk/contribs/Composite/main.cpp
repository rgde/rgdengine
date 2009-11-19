// Compostite.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

//#include "bind_test_stuff.h"
//#include "stl_helpers.h"
//#include "composite.h"

#include "details.h"
#include "type_info.h"
#include "storage.h"


struct test_base 
{

};

struct test : public test_base
{	
	float func0() {return 1.0f;}
	float func1(float f) {return 1.0f*f;}
	float func2(float f, int i) {return 1.0f*f+i;}
	double func3(float f, int i, double d) {return (1.0f*f+i)*d;}

	double get_data() const {return my_private_data;}
	void set_data(double d) {my_private_data = d;}

public:
	int			value;
	std::string name;

private:
	double my_private_data;
};


//template <class T>
//struct mem_func_types_holder
//{
//	//void (T::* ptfptr) (int) = &X::f;
//};

int _tmain(int argc, _TCHAR* argv[])
{
	meta::storage storage;

	storage.get_module("engine")
		.class_<test>("test")
		// можно задать сколько угодно базовых типов. проблема в том что не ясно откуда его получать...
		// видимо тайп инфо должен иметь ссылку на module, а там ссылку на storage...
		.base<test_base>() 
		.def_readonly("name", &test::name)
		.def("value", &test::value)
		.def("func0", &test::func0)
		.def("func3", &test::func3)
		.def_property("data", &test::get_data, &test::set_data)
		.def_property("data_readonly", &test::get_data);

	// так же можно и так:
	//storage.get_module("engine").def_class<test>("test");

	meta::type ti = storage.get_module("engine").types["test"];

	bool has_parents = !ti->parrent_types.empty();
	const char* name = ti->info.name();
	const std::string& type_name = ti->type_name;

	__asm nop;

	// что еще надо:
	//1
	// бинд конструкторов
	// бинд статик методов

	//2
	// сохранние бид информации. 	
	// наследование

	//3
	// серелизация (bin,xml)
	// удобный доступ из С++

	//4 
	// рефлекшен в скрипт

	//5
	// рефлекшен в редактор

	//6
	//механизм визитора для привязки к скрипту в редактор по мета данным (!)

	
	return 0;
}

