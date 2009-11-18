#pragma once

#include <rgde/base/singelton.h>
#include <rgde/io/path.h>

namespace io
{
	typedef boost::shared_ptr<class base_file_souce> file_source_ptr;
	typedef boost::shared_ptr<class read_stream> readstream_ptr;

	class base_file_souce
	{
	public:
		virtual ~base_file_souce(){}

		virtual int			get_priority() const = 0;
		virtual readstream_ptr find(const std::string& file_path) const = 0;
		virtual bool		is_exist	(const std::string& file_path) const = 0;

		static file_source_ptr  create_source(const Path& path);
	};

	class file_system
	{
	public:
		file_system();
		~file_system();
	
		const Path&	get_root_dir() const;
		void		set_root_dir(const Path& path);

		void		add_file_source(const file_source_ptr& spFileSource);
		readstream_ptr find(const std::string& file_path) const;
		bool		is_exist	(const std::string& file_path) const;

		static file_system& get();

	public:
		typedef std::vector<file_source_ptr> sources_vector;
		sources_vector m_sources;
		Path	m_root_path;

		static file_system* ms_instance;
	};

	class scope_path
	{
	public:
		scope_path(const std::string& new_path);
		~scope_path();

	private:
		Path m_old_path;
	};

	class path_add_scoped
	{
	public:
		path_add_scoped(const std::string& new_path);
		~path_add_scoped();

	private:
		Path m_old_path;
	};


	template<typename DataType>
	void stream_to_vector(std::vector<DataType>& v, readstream_ptr s, bool from_current_pos = false)
	{
		if (!s) 
		{
			return;
		}

		uint size = s->get_size();

		if (!from_current_pos)
		{
			s->set_pos(0);
		}
		else
		{
			size -= s->get_pos();
		}
		
		uint vsize  = size/sizeof(DataType);

		v.resize(vsize);
		s->read((byte*)&(v[0]), size);
	}
}