#pragma once

#include <rgde/base/singelton.h>
#include <rgde/io/path.h>

namespace io
{
	typedef boost::shared_ptr<class IFileSource> PFileSource;
	typedef boost::shared_ptr<class IReadStream> PReadStream;

	class IFileSource
	{
	public:
		virtual ~IFileSource(){}

		virtual int			getPriority() const = 0;
		virtual PReadStream findFile(const std::string& strFilePath) const = 0;
		virtual bool		isExist	(const std::string& strFilePath) const = 0;

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
		PReadStream findFile(const std::string& strFilePath) const;
		bool		isExist	(const std::string& strFilePath) const;

	public:
		typedef std::list<PFileSource> Sources;
		Sources m_sources;
		Path	m_rootPath;
	};

	typedef base::TSingelton<CFileSystem> TheFileSystem;

	class ScopePathChange
	{
	public:
		ScopePathChange(const std::string& strNewPath);
		~ScopePathChange();

	private:
		Path m_oldPath;
	};

	class ScopePathAdd
	{
	public:
		ScopePathAdd(const std::string& strNewPath);
		~ScopePathAdd();

	private:
		Path m_oldPath;
	};


	template<typename DataType>
	void StreamToVector(std::vector<DataType>& v, PReadStream& s, bool from_current_pos = false)
	{
		if (!s) 
		{
			return;
		}

		uint size = s->getSize();

		if (!from_current_pos)
		{
			s->setPos(0);
		}
		else
		{
			size -= s->getPos();
		}
		
		uint vsize  = size/sizeof(DataType);

		v.resize(vsize);
		s->readbuff((byte*)&(v[0]), size);
	}
}