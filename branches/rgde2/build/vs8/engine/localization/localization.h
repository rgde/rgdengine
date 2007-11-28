#pragma once

#include <engine/core/errors.h>

namespace rgde
{
	namespace localization		
	{
		struct base_localization_table
		{
			virtual ~base_localization_table() {}
			///virtual const std::wstring& get_text(const std::string& text_id_string) const = 0;

			// temporary
			virtual const std::wstring& get_text(const std::string& text_id_string) const 
			{
				throw core::errors::not_implemented("virtual const std::wstring&  base_localization_table::get_text(const std::string& text_id_string) const");
			}
		};

		typedef boost::shared_ptr<base_localization_table> localization_table_ptr;

		// любая подсистема движка, которой нужно иметь дело с языковой информацией
		// должна содержать в себе такую таблицу
		class localization_table : public base_localization_table
		{
		public:
			typedef std::map<std::string, std::wstring> string_table;
			typedef string_table::const_iterator string_table_it;

			localization_table();

			// вернет пустую строку если id не найден, альтернатива - вернуть text_id_string
			const std::wstring& get_text(const std::string& text_id_string) const;

		protected:
			string_table m_table;
			const std::wstring m_empty_string;
		};

		class localization_system
		{
		public:
			// здесь грузятся таблицы локализаций все какие есть в файле локали.
			localization_system(const std::string& default_locale = "en");

			const std::string& get_locale() const {return m_current_locale;}
			void set_locale(const std::string& new_locale);

			localization_table_ptr get_table(const std::string& table_name);

		protected:
			virtual void reload_locale();

		protected:
			typedef std::map<std::string, localization_table_ptr> loc_tables;
			localization_table_ptr m_empty_table;
			loc_tables m_tables;

			std::string m_current_locale;
		};
	}
}