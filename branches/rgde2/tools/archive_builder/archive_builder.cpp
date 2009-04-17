// archive_builder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "archive.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/any.hpp>

namespace rgde
{
	namespace meta
	{
		struct type_info;

		class property
		{
			property(const property&);

		public:
			enum acsess_type{readonly = 10, writeonly = 20, read_write = 30};

			acsess_type type;
			std::wstring name;
			std::wstring desc;

			template<class OwnerType, typename ValueType>
			property()
			{
			}

			~property() {delete m_holder;}

			template<typename T>
			bool operator=(const T& value)
			{
				return false;
			}

			std::string to_string() const {return std::string();}

		private:
			struct holder
			{
				virtual std::type_info& owner_type() const = 0;
				virtual std::type_info& value_type() const = 0;
			};

			holder* m_holder;
		};

		namespace details
		{
			struct property_impl : public property
			{

			};
		}

		typedef std::vector<property*> properties_vector;
		typedef std::vector<type_info*> type_info_vector;

		struct type_info
		{
			typedef boost::function<void(const std::string&)> getter_func;
			typedef boost::function<const std::string&(void)> setter_func;

			properties_vector properties;
			type_info_vector parent_types;

			std::wstring name;
			std::wstring desc;

			setter_func setter;
			getter_func getter;
		};

		namespace details
		{
			template <class Owner, class Value>
			struct type_info_impl : public type_info
			{
				typedef typename Owner owner_type;
				typedef typename Value value_type;

				typedef boost::function<void(owner_type* owner, const std::string&)> getter_func;
				typedef boost::function<const std::string&(const owner_type*)> setter_func;
			};

			template <class Owner, class Value>
			struct property_adaptor
			{
			public:
				typedef typename type_info_impl<Owner, Value> property_type;

				//property_adaptor(Owner* owner, property_type* pprop)
			

			protected:
				Owner* m_owner;
				property_type *m_property;
			};
		}

		class object
		{
			friend class factory;
		public:
			virtual ~object() {}

			const type_info& get_type_info() const 
			{
				assert(m_type_info);
				return *m_type_info;
			}

		private:
			const type_info* m_type_info;
		};
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	{
		//tree_actor* actor = ...;
		//meta::properties_vector properties = meta::types<tree_actor>::type_info.get_properties(actor);
		//meta::property name_property = properties["name"];
		//name_property = "BigSparklyTree";
	}

	std::wstring start_path = L".";
	std::string fs_filename = "test.fs";

	using namespace rgde;
	namespace fs = rgde::filesystem;

	fs::archive::build(start_path, L"test.archive");

	fs::archive archive(L"test.archive");

	const fs::archive::archive_entry* entry = archive.find_entry(archive.get_entries()[0].name);

	std::vector<char> data;
	bool is_done = archive.read(entry, 0, entry->size, data);

	boost::filesystem::ifstream in(entry->name, std::ios::binary);
	int test_size = boost::filesystem::file_size(entry->name);
	std::vector<char> test_data(test_size);
	in.read(&test_data.front(), test_size);
	int res = memcmp(&data.front(), &test_data.front(), data.size());

	return 0;
}

