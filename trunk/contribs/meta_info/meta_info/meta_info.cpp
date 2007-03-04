#include "stdafx.h"

#include "storage.h"

//#include <boost/python.hpp>

namespace test
{
	class test_class : public meta::object
	{
		float value;
	public:
		test_class(float i = 5) : value(i) {std::cout << "test_class created." << std::endl;}
		float getValue() {return value;}
		void setValue(float i) {value = i;}

		static void register_class_meta(meta::property_storage& storage)
		{
			using boost::bind;
			storage.register_property<test_class, float>(L"value", 
						bind(&test_class::getValue, _1)/*, 
						bind(&test_class::setValue, _1, _2)*/
					);
		}

	protected:
		SUPPORT_RTTI(test_class)
	};

	namespace derived
	{
		class test_child : public test_class
		{
			int value2;
		public:
			test_child(int i = 6) : test_class(), value2(i) {std::cout << "test_child created." << std::endl;}
			int getValue2() {return value2;}
			void setValue2(int i) {value2 = i;}

			static void register_class_meta(meta::property_storage& storage)
			{
				//struct World
				//{
				//	World(std::string msg): msg(msg) {} // added constructor
				//	void set(std::string msg) { this->msg = msg; }
				//	std::string greet() { return msg; }
				//	std::string msg;
				//};

				//class_<World>("World")
				//	.def("greet", &World::greet)
				//	.def("set", &World::set)
				//	;

				//class_<World>("World", init<std::string>())
				//	.def("greet", &World::greet)
				//	.def("set", &World::set)
				//	;

				//class_<World>("World", init<std::string>())
				//	.def(init<double, double>())
				//	.def("greet", &World::greet)
				//	.def("set", &World::set)
				//	;

				// NO CONSTRUCTORS !! [Abstract class]
				//class_<Abstract>("Abstract", no_init)

				//class_<Var>("Var", init<std::string>())
				//	.def_readonly("name", &Var::name)
				//	.def_readwrite("value", &Var::value);

				//struct Var
				//{
				//	Var(std::string name) : name(name), value() {}
				//	std::string const name;
				//	float value;
				//};
				//using boost::bind;
				storage.register_property<test_child, int>
					(L"value2", bind(&test_child::getValue2, _1), bind(&test_child::setValue2, _1, _2));

				//class_<test_child>("test_child")
				//	.def_property("Value2", &test_child::getValue2, &test_child::setValue2);

				//struct Num
				//{
				//	Num();
				//	float get() const;
				//	void set(float value);
				//	...
				//};
				//class_<Num>("Num")
				//	.add_property("rovalue", &Num::get)
				//	.add_property("value", &Num::get, &Num::set);

				//class_<Derived, bases<Base> >("Derived")

				//struct BaseWrap : Base, wrapper<Base>
				//{
				//	int f()
				//	{
				//		return this->get_override("f")();
				//	}
				//};

				//class_<BaseWrap, boost::noncopyable>("Base")
				//	.def("f", pure_virtual(&Base::f))
				//	;
			}

		protected:
			SUPPORT_RTTI(test_child)
		};
	}
}


void print_types_info(meta::types_info_storage& storage)
{
	using std::cout;
	using std::endl;

	using namespace meta;

	const types_map& types = storage.get_types();

	for (types_map::const_iterator it = types.begin(); it != types.end(); ++it)
	{
		type_information_ptr ti = it->second;

		cout << "class: " << ti->type_name;

		types_map& parrents = ti->parrent_types;

		if (!parrents.empty())
		{
			cout << " : ";

			for (types_map::iterator it = parrents.begin(); it != parrents.end(); ++it)
			{
				type_information_ptr& ti = it->second;
				cout << ti->type_name << ", ";
			}			
		}
		cout << endl;


		properties_list plist = ti->get_all_properties();

		if (!plist.empty())
		{
			cout << "properties: ";
			for (properties_list::iterator it = plist.begin(); it != plist.end(); ++it)
			{
				property_ptr& p = *it;

				const type_info& t = p->owner_type;

				cout << get_type_name(p->value_type) << " ";
				std::wcout << p->name.c_str();

				cout << "{";
				if (p->can_read())
					cout << "get, ";

				if (p->can_write())
					cout << "set";
				cout << "}";

				cout << ", ";
			}

			cout << endl;
		}

		cout << endl;
	}	
}

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;
	using namespace meta;

	types_info_storage storage;

	storage.add<test::test_class>();
	storage.add<test::derived::test_child, test::test_class>();




	
	print_types_info(storage);

	cout << ">";

	std::string type_name;
	std::cin >> type_name;
	object_ptr obj = storage.get_factory().create(type_name);
	//object_ptr obj = storage.get_factory().create("test::test_class");

		type_information_ptr ti = obj->get_type_info(storage);

	_asm nop;

	return 0;
}