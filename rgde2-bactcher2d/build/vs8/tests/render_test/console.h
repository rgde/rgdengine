#pragma once

namespace rgde
{
	namespace game
	{
		enum shell_type 
		{
			ST_ILLEGAL  ,
			ST_POINTER  ,
			ST_FUNCTION ,
			ST_ARRAY    ,
			ST_VOID     ,
			ST_INT	    ,
			ST_VECTOR   ,
			ST_FLOAT    ,
			ST_STDSTRING,
			ST_BOOL,
		};

		enum ConsoleMessageType
		{
			CMT_CONSOLE,
			CMT_GAME,
			CMT_CHAT
		};

		class console
		{
		public:
			console();
			~console();

			// clears all console text
			void Clear();
			void Hide()
			{
				is_console_on = false;
				m_is_chat_line_on = false;
			}

			void Show(bool ChatLineOnly = false)
			{
				is_console_on = true;
				m_is_chat_line_on = ChatLineOnly;
			}

			//IronBunny - for render only chat lines
			void ChatLineOnly()
			{
				is_console_on = false;
				m_is_chat_line_on = true;
			}

			bool IsChatLineOn(){return m_is_chat_line_on;}
			void ClearEditingLine(){editing_line="";cursor_pos=0;}
			/// Current Line

			// adds entry to the console's history list of previously displayed lines
			void AddHistoryLine(std::string& line, math::color color = math::color(0,220,0,255));	
			void AddHistoryLine(ConsoleMessageType type,int team,std::string& line, math::color color = math::color(0,220,0,255));	

			/// Windows Messages 
			void MsgKeyDown(unsigned int wParam);
			void MsgChar(unsigned int wParam);
			void ToggleConsole();

			// Draws the console system onto the canvas
			void Render(class Canvas* canvas);
			// initialized during game init process
			void Initialize(const std::string& frontTex, const std::string& backTex);

			bool is_on()
			{ 
				return is_console_on || is_was_toggled; 
			}

			// prints a line of text to the console
			void printf(const char *fmt, ...);
			// prints a colored line of text to the console
			void printf(math::color color,const char *fmt, ...);
			void printf(ConsoleMessageType type,int team,math::color color,const char *fmt, ...);

			// Registers a variable or function for access within the console
			void declare(shell_type type, std::string name, void* pSymbol) { shell.DeclareSymbol(type,name,pSymbol); }
			void declare(const std::string& name, const boost::function<void(const char*)>& function) { shell.DeclareFunction(name, function); }
			void undeclare(const std::string& name) { shell.Undeclare(name); }

			void ExecuteCommand(std::string command){shell.ExecuteSymbol(command);}

			void ExecuteFile(const char* filename);
		private:
			/// A symbol defined for use in shell.
			struct shell_symbol 
			{				
				shell_type type;   /// type of the symbol				
				std::string name;	/// symbol name				
				void *value;	/// symbol value
				boost::function<void(const char*)> function;
			};

			struct history_line
			{
				int type;
				int team;
				std::wstring text;
				math::color color;
			};
			//
			/// shell is a class for storing and executing data and functions
			//
			class shell
			{
			public:
				// registerd a var or function for use with the console exceution system
				void DeclareSymbol(shell_type type, const std::string& name, void* pSymbol);
				void DeclareFunction(const std::string& name, const boost::function<void(const char*)>& function);
				void Undeclare(const std::string& name);
				// Parses & executes command, whether a function with params or setting of a var
				std::string ExecuteSymbol(std::string cmd);
				// Returns string name of symbol, function or var, on the array
				std::string get_symbol_name(size_t index){ return symbols[index].name; }
				size_t get_num_symbols() const { return symbols.size(); }

			private:
				std::vector<shell_symbol> symbols;
				// Finds a specified command symbol within the execution shell, returns the index on the shell_symbol list
				int FindCmdName(std::string s);
				// Returns symbol value if a var, converted to string
				std::string GetSymbolValue(shell_symbol symbol);
			};

			/// Tab key; shows commands
			void KeyTab();	

			/// Return key is pressed, line is processed
			void KeyReturn();				
			/// Up key; previous line is shown
			void KeyArrowUp();				
			/// Down key; scrolls to most recent line
			void KeyArrowDown();			
			/// Delete key; deletes character at cursor pos
			void KeyBackspace();

		private:
			/// Old editing line while user scrolls up and down
			std::string old_edit_line;	
			/// Current history line on (for up/down arrows)
			int cur_history_line;		
			/// State of console
			bool is_console_on;
			bool is_was_toggled;

			/// Textures
			render::texture_ptr textures[2];

			bool m_is_chat_line_on;

			std::string editing_line;
			/// Command shell for console
			shell shell;	
			/// Position of blinking text cursor
			int cursor_pos;	

			/// Previous Lines
			std::vector<history_line> m_history_lines; 
			/// Recent lines to draw directly to viewport
			int m_recent_lines;				  
			/// animated fade factor for 'disappearing' history line
			float m_fade_line_opacity_factor; 

			bool bDrawChatLines;

			bool m_is_allow_toggling;

			/// Previous TYPED lines
			std::vector<std::string> m_typed_history;;
		};
	}
}