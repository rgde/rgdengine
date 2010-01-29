#pragma once

struct lua_State;

namespace gui
{
	class ScriptSystem;
	class ScriptStack;

	class  ScriptObject
	{
		ScriptObject();
		ScriptObject(const ScriptObject&);
	public:
		ScriptObject(ScriptSystem& ref) : m_ref_script(ref) {}
		virtual ~ScriptObject() {}

	protected:
		virtual void thisset() = 0;
		void thisreset();
		ScriptSystem&	m_ref_script;
		friend class ScriptStack;
	};

	class ScriptStack
	{
	public:
		ScriptStack()
		{
			m_stack.reserve(16);
		}
		void clear()
		{
			m_stack.clear();
		}
		void push(ScriptObject* obj)
		{
			assert(obj);
			obj->thisset();
			m_stack.push_back(obj);
		}
		void pop()
		{
			if(m_stack.size())
			{
				size_t size = m_stack.size();
				if(size == 1)
					m_stack[size - 1]->thisreset();

				m_stack.pop_back();
				size = m_stack.size();
				if(size)
					m_stack[size - 1]->thisset();
			}
		}
	private:
		std::vector<ScriptObject*> m_stack;
	};

	class  ScriptSystem
	{
	public:
		ScriptSystem();
		~ScriptSystem();

		lua_State* LuaState();

		bool ExecuteString(const std::string& script, ScriptObject* obj, const std::string& filename = "unknown event");
		bool ExecuteString(const std::string& script, const std::string& filename = "mem buffer");
		bool ExecuteFile(const std::string& filename);

		const std::string& GetLastError() { return m_error; }

	protected:
		std::string LoadFile(const std::string& filename);
		std::string GetLuaError();
		bool Execute(const std::string& script, const std::string& filename);
	
	private:
		lua_State* m_state;
		std::string m_error;

		ScriptStack m_thisStack;
	};
}