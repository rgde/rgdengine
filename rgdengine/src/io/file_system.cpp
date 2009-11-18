#include "precompiled.h"

#include <rgde/io/file_system.h>
#include <rgde/io/file.h>

namespace
{
	bool operator<(const io::file_source_ptr& s1, const io::file_source_ptr& s2)
	{
		return s1->get_priority() < s2->get_priority();
	}
}

namespace io
{
	class directory_source : public base_file_souce
	{
	public:
		directory_source(const Path &path)
			: m_path(path)
		{
		}
		~directory_source()
		{
		}

		int get_priority() const
		{
			return 100;
		}

		readstream_ptr find(const std::string& file_path) const
		{
			readstream_ptr	s(new read_file(file_path));

			if (s->is_valid()) return s;

			return readstream_ptr();
		}

		bool is_exist(const std::string& file_path) const
		{
			return read_file(file_path).is_open();
		}

	private:
		Path	m_path;
	};

	file_source_ptr base_file_souce::create_source(const Path &path)
	{
		return file_source_ptr(new directory_source(path));
	}


	file_system::file_system()
		: m_root_path("./Media/")
	{
		add_file_source(file_source_ptr(new directory_source(Path(""))));
	}

	const Path& file_system::get_root_dir() const
	{
		return m_root_path;
	}

	void file_system::set_root_dir(const Path &path)
	{
		m_root_path = path;
	}

	void file_system::add_file_source(const file_source_ptr& spFileSource)
	{
		m_sources.push_back(spFileSource);
		std::sort(m_sources.begin(), m_sources.end());
	}

	readstream_ptr file_system::find(const std::string& file_path) const
	{
		readstream_ptr s;
		std::string total_path	= m_root_path.string();
		if(total_path != "")
		{
			char last = total_path[total_path.size() - 1];
			if((last != '/') &&
			   (last != '\\'))
			total_path += "/";
		}
		total_path += file_path;

		for (sources_vector::const_iterator it = m_sources.begin(); it != m_sources.end(); ++it)
		{
			s = (*it)->find(total_path);
			if (s)
				break;
		}

		return s;
	}

	bool file_system::is_exist(const std::string& file_path) const
	{
		bool result	= false;
		std::string total_path	= m_root_path.string() + "/" + file_path;

		for (sources_vector::const_iterator it = m_sources.begin(); it != m_sources.end(); ++it)
		{
			result = (*it)->is_exist(total_path);
			if (result)
				break;
		}

		return result;
	}

	scope_path::scope_path(const std::string& new_path)
		: m_old_path(TheFileSystem::get().get_root_dir())
	{
		TheFileSystem::get().set_root_dir(Path(new_path));
	}

	scope_path::~scope_path()
	{
		TheFileSystem::get().set_root_dir(m_old_path);
	}


	path_add_scoped::path_add_scoped(const std::string& new_path)
		: m_old_path(TheFileSystem::get().get_root_dir())
	{
		std::string total_path	= m_old_path.string() + "/" + new_path;
		TheFileSystem::get().set_root_dir(Path(total_path));
	}

	path_add_scoped::~path_add_scoped()
	{
		TheFileSystem::get().set_root_dir(m_old_path);
	}
}