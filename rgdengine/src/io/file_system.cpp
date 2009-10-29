#include "precompiled.h"

#include <rgde/io/file_system.h>
#include <rgde/io/file.h>

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
			readstream_ptr	s(new read_file_stream(file_path));

			if (s->is_valid()) return s;

			return readstream_ptr();
		}

		bool isExist(const std::string& file_path) const
		{
			return read_file_stream(file_path).isOpened();
		}

	private:
		Path	m_path;
	};

	PFileSource base_file_souce::CreateDirectorySource(const Path &path)
	{
		return PFileSource(new directory_source(path));
	}


	CFileSystem::CFileSystem()
		: m_rootPath("./Media/")
	{
		addFileSource(PFileSource(new directory_source(Path(""))));
	}

	const Path& CFileSystem::getRootDir() const
	{
		return m_rootPath;
	}

	void CFileSystem::setRootDir(const Path &path)
	{
		m_rootPath = path;
	}

	namespace functors
	{
		struct PrioritySorter
		{
			bool operator()(const PFileSource &s1, const PFileSource &s2)
			{
				return s1->get_priority() < s2->get_priority();
			}
		};
	}	

	void CFileSystem::addFileSource(const PFileSource& spFileSource)
	{
		m_sources.push_back(spFileSource);
		//std::sort(m_sources.begin(), m_sources.end(), functors::PrioritySorter());
	}

	readstream_ptr CFileSystem::find(const std::string& file_path) const
	{
		readstream_ptr s;
		std::string total_path	= m_rootPath.string();
		if(total_path != "")
		{
			char last = total_path[total_path.size() - 1];
			if((last != '/') &&
			   (last != '\\'))
			total_path += "/";
		}
		total_path += file_path;

		for (Sources::const_iterator it = m_sources.begin(); it != m_sources.end(); ++it)
		{
			s = (*it)->find(total_path);
			if (s)
				break;
		}

		return s;
	}

	bool CFileSystem::isExist(const std::string& file_path) const
	{
		bool result	= false;
		std::string total_path	= m_rootPath.string() + "/" + file_path;

		for (Sources::const_iterator it = m_sources.begin(); it != m_sources.end(); ++it)
		{
			result = (*it)->isExist(total_path);
			if (result)
				break;
		}

		return result;
	}

	ScopePathChange::ScopePathChange(const std::string& strNewPath)
		: m_oldPath(TheFileSystem::get().getRootDir())
	{
		TheFileSystem::get().setRootDir(Path(strNewPath));
	}

	ScopePathChange::~ScopePathChange()
	{
		TheFileSystem::get().setRootDir(m_oldPath);
	}


	ScopePathAdd::ScopePathAdd(const std::string& strNewPath)
		: m_oldPath(TheFileSystem::get().getRootDir())
	{
		std::string total_path	= m_oldPath.string() + "/" + strNewPath;
		TheFileSystem::get().setRootDir(Path(total_path));
	}

	ScopePathAdd::~ScopePathAdd()
	{
		TheFileSystem::get().setRootDir(m_oldPath);
	}
}