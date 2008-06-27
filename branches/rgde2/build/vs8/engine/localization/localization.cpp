#include "stdafx.h"

#include "localization.h"

namespace rgde
{
	namespace localization
	{
		localization_table::localization_table()
		{
		}

		const std::wstring& localization_table::get_text(const std::string& id_string) const
		{
			string_table_it it = m_table.find(id_string);
			return it != m_table.end() ? it->second : m_empty_string;
		}

		void localization_table::clear()
		{
			m_table.clear();
		}

		void localization_table::add_text(const std::string& id_string, const std::wstring& text)
		{
			m_table[id_string] = text;
		}

		//////////////////////////////////////////////////////////////////////////

		localization_system::localization_system(const std::string& default_locale)
		{
			set_locale(default_locale);
		}

		void localization_system::set_locale(const std::string& new_locale)
		{
			m_current_locale = new_locale;
			m_current_tables = m_tables[m_current_locale];
		}

		localization_table_ptr localization_system::get_table(const std::string& locale, const std::string& table_id)
		{
			loc_tables_iter it = m_tables.find(locale);
			
			if (it != m_tables.end())
			{
				return it->second[table_id];
			}
			else
				return m_empty_table;
		}

		localization_table_ptr localization_system::add_table(const localization_table_ptr& table, const std::string& locale,
															  const std::string& table_id )
		{
			m_tables[locale][table_id] = table;
			m_current_tables = m_tables[m_current_locale];
			return table;
		}

		const std::wstring& localization_system::get_text(const std::string& id_string, const std::string& id_table) const
		{
			map_tables_citer it = m_current_tables.find(id_table);
			if (it != m_current_tables.end() && it->second)
			{
				return it->second->get_text(id_string);
			}
	
			const static std::wstring s_empty_string;
			return s_empty_string;
		}
	}
}