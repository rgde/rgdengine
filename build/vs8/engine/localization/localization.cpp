#include "stdafx.h"

#include "localization.h"

namespace rgde
{
	namespace localization
	{
		localization_table::localization_table()
		{
		}

		const std::wstring& localization_table::get_text(const std::string& text_id_string) const
		{
			string_table_it it = m_table.find(text_id_string);
			return it != m_table.end() ? it->second : m_empty_string;
		}

		//////////////////////////////////////////////////////////////////////////

		localization_system::localization_system(const std::string& default_locale)
		{
			m_empty_table.reset(new base_localization_table());
			set_locale(default_locale);
		}

		void localization_system::set_locale(const std::string& new_locale)
		{
			m_current_locale = new_locale;
		}

		localization_table_ptr localization_system::get_table(const std::string& table_name)
		{
			return m_empty_table;
		}

		void localization_system::reload_locale()
		{
		}
	}
}