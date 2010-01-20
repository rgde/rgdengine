#include "stdafx.h"
#include "scripthost.h"

namespace gui
{

void ScriptObject::thisreset()
{
	if(m_ref_script.LuaState())
		luabind::globals(m_ref_script.LuaState())["this"] = 0;
}

ScriptSystem::ScriptSystem()
: m_state(0)
{
	m_state = lua_open();
	if (m_state)
	{
		luaL_openlibs(m_state);

		using namespace luabind;
		open(m_state);
		module(m_state)
		[
			class_<ScriptObject>("ScriptObject")
		];

	}
}
ScriptSystem::~ScriptSystem()
{
	lua_close (m_state);
}

lua_State* ScriptSystem::LuaState()
{
	return m_state;
}

bool ScriptSystem::ExecuteString(const std::string& script, ScriptObject* obj, const std::string& filename)
{
	bool retval = false;
	if(obj)
	{
		m_thisStack.push(obj);
		retval = ExecuteString(script, filename);
		m_thisStack.pop();
	}
	else
	{
		m_error = "An empty object passed to the script. ";
	}
	return retval;
}

bool ScriptSystem::ExecuteString(const std::string& script, const std::string& filename)
{
	bool success = false;
	if(m_state)
	{
		if(int result = luaL_loadbuffer(m_state, script.c_str(), script.length(), filename.c_str()))
		{
			m_error = "Can't parse LUA string: ";
			m_error += GetLuaError();
			return false;
		}

		try
		{
			success = Execute(script, filename);
		}
		catch(std::exception& e)
		{
			m_error = "LUA runtime error: ";
			m_error += e.what();
		}
	}
	else
	{
		m_error = "LUA state is uninitialized. ScriptSystem isn't available. ";
	}
	return success;
}

bool ScriptSystem::Execute(const std::string& script, const std::string& filename)
{
	if(int result = lua_pcall(m_state, 0, 0, 0))
	{
		m_error = "Can't execute LUA string: ";
		m_error += GetLuaError();
		return false;
	}
	return true;
}

bool ScriptSystem::ExecuteFile(const std::string& filename)
{
	const std::string& script = LoadFile(filename);
	if(!script.empty())
	{
		m_error = "The file '";
		m_error += filename;
		m_error += "' cannot be loaded. ";
		return ExecuteString(script, filename);
	}
	
	return true; // just empty file
}

std::string ScriptSystem::LoadFile(const std::string& filename)
{
	std::string str;
	FILE* file = fopen(filename.c_str(), "rb");
	if(file)
	{
		fseek(file, 0, SEEK_END);
		long length = ftell(file);
		fseek(file, 0, SEEK_SET);
		if (length > 0)
		{
			char* s = new(std::nothrow) char[length + 1];
			if(s)
			{
				s[length] = 0;
				size_t read = fread(s, (size_t)length, 1, file);
				if (read == 1)
				{
					str = s;
				}
				delete [] s;
			}			
		}
		fclose(file);
	}
	return str;
}

std::string ScriptSystem::GetLuaError()
{
	std::string ret = "<unknown>";
	if(m_state && lua_isstring(m_state, -1))
	{
		ret = lua_tostring(m_state, -1);
		lua_pop(m_state, 1);
	}
	return ret;
}

}