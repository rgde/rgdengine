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


//
//<effect length="20" name value="big-boom-bam" description="firebol_explode">
//<Params>
//<rotation type="float3" script="rand(0,0,20);rand(1,0,20);rand(2,0,20)" value="0;0;0"/>
//<position type="float3" script="" value="0;0;0"/>
//</Params>
//<item type="ComsiteItem" >
//<SubParts>
//<Start>
//<item type="StaticParticles" file="fire.prt" start="0" start_frame="0" end_frame="7" cycling="0" additive_blend="0"/>
//<item type="StaticParticles" file="stars.prt" start_frame="0" end_frame="7" cycling="0" additive_blend="0"/>
//</Start>
//
//<Casting cycling="1" cycling_delta="0.20" fade_time="0.25">
//<item type="StaticParticles" file="fire.prt" start_frame="7" rend_frame="7" cycling="0" additive_blend="0"/>
//<item type="StaticParticles" file="stars.prt" start_frame="7" rend_frame="7" cycling="0" additive_blend="0"/>
//</Casting>
//
//<End>
//<item type="StaticParticles" file="fire.prt" rstart_frame="7" rend_frame="0" cycling="0" additive_blend="0"/>
//<item type="StaticParticles" file="stars.prt" rstart_frame="7" rend_frame="0" cycling="0" additive_blend="0"/>
//</End>
//</SubParts>
//<Body>
//
//</Body>		
//</item>
//
//<item type="StaticParticles" file="fire.prt" start="0" end="20" cycling="1" additive_blend="0">
//<item type="StaticParticles" file="stars.prt" start="0" end="20" cycling="1" additive_blend="0"/>
//<item type="StaticParticles" file="fire.prt" start="0" end="20" cycling="1" additive_blend="1"/>
//<!--	<item type="StaticParticles" file="flare.prt" start="0" end="2" cycling="0" additive_blend="1"/> -->
//</effect>
//


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
		virtual ~abstract_command(){}
		virtual boost::any execute(const command&) = 0;
	};

	template<typename result_type>
	struct t_func : public abstract_func
	{
		boost::function<result_type(const command&)> func;
		boost::any execute(const command& c) { return func(c); }
	};

	typedef boost::shared_ptr<abstract_func> func_ptr;
}

struct command_processor
{
	command_processor(){}
	~command_processor(){}

	typedef boost::function<std::string (const command&)> command_handler;
	typedef std::map<std::string, command_handler> Commands;

	Commands commands;

	struct command_info
	{
		std::string name;
		funcs_helpers::func_ptr func;
		boost::any execute(const command& c) {return func ? func->execute(c) : boost::any();}
	};

	void bind(const std::string& command_name, command_handler handler)
	{
		if (commands.find(command_name) == commands.end())
		{
			commands[command_name] = handler;
		}
	}

	template<class FunctionType, typename result_type>
	result_type execute(const command& c, FunctionType func)
	{
		if (c.args.size() != FunctionType::args)
		{
			std::cerr << "Invalid number of argiments!" << std::endl;
			return result_type();
		}
		return result_type();
	}

	template<class FunctionType>
	void _bind(const std::string& command_name, FunctionType func)//, ...)
	{
		using std::cout;
		using std::endl;

		cout << FunctionType::args << endl;
		cout << typeid(FunctionType::result_type).name() << endl;
		cout << typeid(FunctionType::arg1_type).name() << endl;
		cout << typeid(FunctionType::arg2_type).name() << endl;

		typedef command_info<float> command_info;
		command_info info;
		info.name = command_name;
		typedef FunctionType F;
		
		info.func = boost::bind(&command_processor::execute<F, F::result_type>, this, _1, func);

		command c(info.name);
		info.func(c);
	}


	std::string execute(const command& c)
	{
		Commands::iterator it = commands.find(c.name);

		if (it != commands.end())
		{
			return it->second(c);
		}
		else
		{
			return std::string("unknown command");
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


int _tmain(int argc, _TCHAR* argv[])
{
	using boost::function_traits;


	typedef boost::function<float(float, float)> test_func;

	
	using boost::bind;

	{
		command_processor p;
		boost::function<float(float, float)> f = bind(&math::rand, _1, _2);
		p._bind("rand", f);
	}
	
	return 0;

	std::string test_string = "rand(1,2);rand(5,7);rand(15,40)";

	std::vector<std::string> commands;
	parser::tokenize(test_string, commands);

	command_processor p;
	p.bind("rand", boost::bind(&bindings::math::rand, _1));

	for (int i = 0; i < commands.size(); ++i)
	{
		command c(commands[i]);
		std::cout << commands[i].c_str() << "=" << p.execute(c).c_str() << std::endl;
	}

	return 0;
}