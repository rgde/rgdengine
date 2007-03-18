// xml_utilities.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

#include <boost/tokenizer.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/type_traits.hpp>
#include <boost/type_traits/function_traits.hpp>

#include <stdarg.h>

struct parser
{
	static void tokenize(const std::string& src, std::vector<std::string>& out_tokens, 
		const std::string& separators = ";");

	static bool parse_command(const std::string& src, std::string& command_name, 
		std::vector<std::string>& params);


	template<typename T>
	static void parse(const std::vector<std::string>& in, std::vector<T>& out)
	{
		out.resize(in.size());
		for (unsigned int i = 0; i < in.size(); ++i)
		{
			out[i] = boost::lexical_cast<T>(in[i]);
		}
	}
};

void parser::tokenize(const std::string& src, std::vector<std::string>& out_tokens, 
					  const std::string& separators)
{	
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(separators.c_str());

	tokenizer t(src, sep);

	for(tokenizer::iterator beg=t.begin(); beg!=t.end();++beg)
	{
		out_tokens.push_back(*beg);
	}
}

struct command
{
	command(const std::string& src)
	{
		std::vector<std::string> tokens;
		parser::tokenize(src, tokens, "(,)");

		if (tokens.size() > 1)
			name = tokens[0];

		if (tokens.size() > 2)
		{
			args.resize(tokens.size()-1);
			std::copy((++tokens.begin()), tokens.end(), args.begin());
		}		
	}

	std::string name;
	typedef std::vector<std::string> Args;
	Args args;
};

namespace funcs_helpers
{
	struct abstract_func
	{
		virtual ~abstract_func(){}
		virtual boost::any execute(const command&) = 0;
	};

	typedef boost::shared_ptr<abstract_func> func_ptr;

	template<typename result_type>
	struct t_func : public abstract_func
	{
		boost::function<result_type(const command&)> func;
		boost::any execute(const command& c) { return func(c); }

		static func_ptr create()
		{
			return func_ptr(new t_func);
		}
	};

	template<>
	struct t_func<void> : public abstract_func
	{
		boost::function<void(const command&)> func;
		boost::any execute(const command& c) 
		{ 
			func(c); 
			return boost::any();
		}

		static func_ptr create()
		{
			return func_ptr(new t_func);
		}
	};
}

namespace details
{
	struct call_helper
	{
		template<typename result_type>
		static result_type call(const command& c, boost::function<result_type(void)>& func)
		{
			std::cerr << "0 param func called" << std::endl;
			func();
		}

		template<typename result_type, typename P1>
		static result_type call(const command& c, boost::function<result_type(P1)>& func)
		{
			std::cerr << "1 param func called" << std::endl;
			return result_type();
		}

		template<typename result_type, typename P1, typename P2>
		static result_type call(const command& c, boost::function<result_type(P1,P2)>& func)
		{
			std::cerr << "2 param func called" << std::endl;
			if (c.args.size() != 2)
			{
				std::cerr << "Error! Incorrect param num, while calling func <" << c.name << ">" << std::endl;
				return result_type();
			}

			try
			{
				P1 p1 = boost::lexical_cast<P1>(c.args[0]);
				P2 p2 = boost::lexical_cast<P2>(c.args[1]);
				return func(p1, p2);
			}
			catch (boost::bad_lexical_cast& ex)
			{
				
				std::cerr << "Invalid pram types!" << std::endl
						<< ex.what() << std::endl;
				return result_type();				
			}			
		}
	};


	template<class FunctionType, typename result_type>
	struct exec_helper
	{
		static result_type call(const command& c, FunctionType func)
		{
			if (c.args.size() != FunctionType::args)
			{
				std::cerr << "Invalid number of argiments!" << std::endl;
				std::cerr << "Number of args is " << FunctionType::args << std::endl;
				return FunctionType::result_type();
			}
			return call_helper::call(c, func);
			//return FunctionType::result_type();
		}
	};

	template<class FunctionType>
	struct exec_helper<FunctionType, void>
	{
		static void call(const command& c, FunctionType func)
		{
			std::cout << "void func called" << std::endl;
			//call_helper::call(c, func);
		}
	};
}

struct command_processor
{
	command_processor(){}
	~command_processor(){}

	struct command_info
	{
		std::string name;
		funcs_helpers::func_ptr func;
		boost::any execute(const command& c) {return func ? func->execute(c) : boost::any();}
	};

	typedef std::map<std::string, command_info> Commands;
	Commands commands;

	template<class FunctionType>
	void bind(const std::string& command_name, FunctionType func)//, ...)
	{
		using std::cout;
		using std::endl;

		//cout << FunctionType::args << endl;
		//cout << typeid(FunctionType::result_type).name() << endl;
		//cout << typeid(FunctionType::arg1_type).name() << endl;
		//cout << typeid(FunctionType::arg2_type).name() << endl;

		//typedef command_info<float> command_info;

		typedef typename FunctionType::result_type result_type;
		typedef funcs_helpers::func_ptr func_ptr;
		typedef funcs_helpers::t_func<result_type> t_func;

		t_func* fp = new t_func;
		//cout << typeid(result_type).name() << endl;
		//boost::function<void(const command&)> tp = boost::bind(&details::exec_helper<FunctionType, result_type>::call, _1, func);
		fp->func = boost::bind(&details::exec_helper<FunctionType, result_type>::call, _1, func);

		command_info info;
		info.name = command_name;
		info.func = func_ptr(fp);

		//command c(info.name);
		//info.func->execute(c);

		commands[info.name] = info;
	}


	boost::any execute(const command& c)
	{
		Commands::iterator it = commands.find(c.name);

		if (it != commands.end())
		{
			return (it->second.func)->execute(c);
		}
		else
		{
			boost::any();//return std::string("unknown command");
		}
	}
};

namespace math
{
	float rand(float min = 0, float max = 1)
	{
		return (max - min) / 2.0f;
	}
}

void testit ( int i, ...)
{
	va_list argptr;
	va_start(argptr, i);

	if ( i == 0 ) {
		int n = va_arg( argptr, int );
		printf( "%d\n", n );
	} else {
		char *s = va_arg( argptr, char* );
		printf( "%s\n", s);
	}
}


namespace bindings
{
	namespace math
	{
		std::string rand(const command& c)
		{
			std::vector<float> params;
			parser::parse(c.args, params);
			float result = ::math::rand(params[0], params[1]);
			std::string out = boost::lexical_cast<std::string>(result);
			return out;
		}
	}
}


std::ostream& operator <<(std::ostream& out, const boost::any& value)
{
	if (value.type() == typeid(float))
	{
		out << boost::any_cast<float>(value);
	}
	return out;
}

float print_help(float, float)
{
	std::cout << "====HELP!!!====" << std::endl;
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	using boost::function_traits;


	//typedef boost::function<float(float, float)> test_func;

	
	using boost::bind;

	//{
	//	command_processor p;
	//	boost::function<float(float, float)> f = bind(&math::rand, _1, _2);
	//	p.bind("rand", f);
	//	//p.bind("rand", &math::rand);
	//}
	//
	//return 0;

	std::string test_string = "rand(1,2);rand(5,7);rand(15,40);";//help();";

	std::vector<std::string> commands;
	parser::tokenize(test_string, commands);

	command_processor p;
	boost::function<float(float, float)> f = bind(&math::rand, _1, _2);
	p.bind("rand", f);
	boost::function<float(float, float)> f1 = bind(&print_help, 1.0f, 2.0f);
	p.bind("help", f1);//bind(&print_help));

	for (size_t i = 0; i < commands.size(); ++i)
	{
		command c(commands[i]);
		std::cout << commands[i].c_str() << "=" << p.execute(c) << std::endl;
	}

	return 0;
}