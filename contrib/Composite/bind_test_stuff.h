#pragma once



class MyCustomShaderData
{
public:
	void set() const {std::cout << "MyCustomShaderData setted." << std::endl;}
	float getFloat(float x) {return 100.0f*x;}
};

struct TestParam
{
	void set(float f) {std::cout << "float param setted:" << f << std::endl;}
	void set(int i) {std::cout << "Int param setted:" << i << std::endl;}
	void set(const MyCustomShaderData& d) {d.set();}
};


template<class T>
struct bind_helper
{
	typedef typename boost::call_traits<T>::param_type param_type;
	typedef boost::function<void(void)> ResultFunc;
	typedef boost::function<param_type ()> GetFunction;

	static ResultFunc create(GetFunction g, TestParam* tp)
	{
		return boost::bind((void (TestParam::*)(param_type))&TestParam::set, tp, (GetFunction)(boost::bind(g), _1));
	}

	static ResultFunc create(param_type p, TestParam* tp)
	{
		return boost::bind((void (TestParam::*)(param_type))&TestParam::set, tp, p);
	}
};

template<class T>
boost::function<void(void)> create_function(const T& p, TestParam* tp)
{
	return bind_helper<T>::create(p, tp);
}

template<class T, class GetFunc>
boost::function<void(void)> create_function(GetFunc gf, TestParam* tp)
{
	return bind_helper<T>::create(gf, tp);
}


void test_bind_stuff()
{
	TestParam tp;
	
	MyCustomShaderData d;

	boost::function<void(void)> f1 = create_function(1.0f, &tp);
	boost::function<void(void)> f2 = create_function(5, &tp);
	boost::function<void(void)> f3 = create_function(d, &tp);

	boost::function<float(void)> get_func = (boost::function<float(void)>)boost::bind(&MyCustomShaderData::getFloat, &d, _1);
	boost::function<void(void)> f4 = create_function<float>(get_func, &tp);

	std::cout << "functors created." << std::endl;

	f1();
	f2();
	f3();
	f4(1.0f);
}