#pragma once

#include <rgde/base/singelton.h>
#include <rgde/io/path.h>

namespace io
{
	typedef boost::shared_ptr<class base_file_souce> PFileSource;
	typedef boost::shared_ptr<class read_stream> readstream_ptr;

	class base_file_souce
	{
	public:
		virtual ~base_file_souce(){}

		virtual int			get_priority() const = 0;
		virtual readstream_ptr find(const std::string& file_path) const = 0;
		virtual bool		is_exist	(const std::string& file_path) const = 0;

		static PFileSource  CreateDirectorySource(const Path& path);
		//static PFileSource CreateZipSource(const Path& path);
	};

	class CFileSystem
	{
	protected:
		CFileSystem();

	public:
		
		const Path&	getRootDir() const;
		void		setRootDir(const Path& path);

		void		addFileSource(const PFileSource& spFileSource);
		readstream_ptr find(const std::string& file_path) const;
		bool		is_exist	(const std::string& file_path) const;

	public:
		typedef std::list<PFileSource> Sources;
		Sources m_sources;
		Path	m_rootPath;
	};

	typedef base::singelton<CFileSystem> TheFileSystem;

	class ScopePathChange
	{
	public:
		ScopePathChange(const std::string& strNewPath);
		~ScopePathChange();

	private:
		Path m_oldPath;
	};

	class path_add_scoped
	{
	public:
		path_add_scoped(const std::string& strNewPath);
		~path_add_scoped();

	private:
		Path m_oldPath;
	};


	template<typename DataType>
	void stream_to_vector(std::vector<DataType>& v, readstream_ptr& s, bool from_current_pos = false)
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