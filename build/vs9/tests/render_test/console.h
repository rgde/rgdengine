#pragma once

#include "sprite.h"

namespace rgde
{
	namespace game
	{
		enum shell_type 
		{
			st_illegal  ,
			st_pointer  ,
			st_function ,
			st_array    ,
			st_void     ,
			st_int	    ,
			st_vector   ,
			st_float    ,
			st_stdstring,
			st_bool,
		};

		enum message_type
		{
			mt_console,
			mt_game,
			mt_chat

		};

		class console
		{
		public:
			console(render::device& dev);
			~console();

			// clears all console text
			void clear();
			void hide();

			void show(bool chat_line_only = false);

			bool is_chat_line_on(){return m_is_chat_line_on;}
			void clear_edit_line(){editing_line.clear();cursor_pos=0;}
			/// Current Line

			// adds entry to the console's history list of previously displayed lines
			void add_history_line(std::string& line, math::color color = math::color(0,220,0,255));	
			void add_history_line(message_type type,int team,std::string& line, math::color color = math::color(0,220,0,255));	

			/// Windows Messages 
			void msg_keydown(unsigned int wparam);
			void msg_char(unsigned int wparam);
			void toggle();

			void render();
			// initialized during game init process
			void init(const std::string& front_tex, const std::string& back_tex);

			bool is_on()
			{ 
				return is_console_on || is_was_toggled; 
			}

			// prints a line of text to the console
			void printf(const char *fmt, ...);
			// prints a colored line of text to the console
			void printf(math::color color,const char *fmt, ...);
			void printf(message_type type,int team,math::color color,const char *fmt, ...);

			// Registers a variable or function for access within the console
			void declare(shell_type type, std::string name, void* pSymbol) { m_shell.decl_symbol(type,name,pSymbol); }
			void declare(const std::string& name, const boost::function<void(const char*)>& function) { m_shell.decl_func(name, function); }
			void undeclare(const std::string& name) { m_shell.undecl(name); }

			void exec_command(std::string command){m_shell.exec_symbol(command);}

			void exec_file(const char* filename);

		private:
			render::device& m_device;

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
				void decl_symbol(shell_type type, const std::string& name, void* pSymbol);
				void decl_func(const std::string& name, const boost::function<void(const char*)>& function);
				void undecl(const std::string& name);
				// Parses & executes command, whether a function with params or setting of a var
				std::string exec_symbol(std::string cmd);
				// Returns string name of symbol, function or var, on the array
				std::string get_symbol_name(size_t index){ return symbols[index].name; }
				size_t get_num_symbols() const { return symbols.size(); }

			private:
				std::vector<shell_symbol> symbols;
				// Finds a specified command symbol within the execution shell, returns the index on the shell_symbol list
				int find_cmd_name(std::string s);
				// Returns symbol value if a var, converted to string
				std::string get_symbol_value(shell_symbol symbol);
			};

			/// Tab key; shows commands
			void key_tab();	

			/// Return key is pressed, line is processed
			void key_return();				
			/// Up key; previous line is shown
			void key_up();				
			/// Down key; scrolls to most recent line
			void key_down();			
			/// Delete key; deletes character at cursor pos
			void key_backspace();

			void set_back_texture(render::texture_ptr t) {textures[0] = t;}
			void set_front_texture(render::texture_ptr t) {textures[1] = t;}

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
			render::font_ptr m_small_font;
			render::font_ptr m_medium_font;

			bool m_is_chat_line_on;

			std::string editing_line;
			/// Command shell for console
			shell m_shell;	
			/// Position of blinking text cursor
			int cursor_pos;	

			/// Previous Lines
			std::vector<history_line> m_history_lines; 
			/// Recent lines to draw directly to viewport
			int m_recent_lines;				  
			/// animated fade factor for 'disappearing' history line
			float m_fade_line_opacity_factor; 

			bool m_draw_chat;

			bool m_is_allow_toggling;

			/// Previous TYPED lines
			std::vector<std::string> m_typed_history;

			render::canvas m_canvas;
		};
	}
}