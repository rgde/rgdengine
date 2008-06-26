#include "precompiled.h"

#include <rgde/io/file_system.h>
#include <rgde/io/file.h>

namespace io
{
	class DirectorySourceImpl : public IFileSource
	{
	public:
		DirectorySourceImpl(const Path &path)
			: m_path(path)
		{
		}
		~DirectorySourceImpl()
		{
		}

		int getPriority() const
		{
			return 100;
		}

		PReadStream findFile(const std::string& strFilePath) const
		{
			PReadStream	s(new CReadFileStream(strFilePath));

			if (s->isValid()) return s;

			return PReadStream();
		}

		bool isExist(const std::string& strFilePath) const
		{
			return CReadFileStream(strFilePath).isOpened();
		}

	private:
		Path	m_path;
	};

	PFileSource IFileSource::CreateDirectorySource(const Path &path)
	{
		return PFileSource(new DirectorySourceImpl(path));
	}


	CFileSystem::CFileSystem()
		: m_rootPath("./Media/")
	{
		addFileSource(PFileSource(new DirectorySourceImpl(Path(""))));
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
				return s1->getPriority() < s2->getPriority();
			}
		};
	}	

	void CFileSystem::addFileSource(const PFileSource& spFileSource)
	{
		m_sources.push_back(spFileSource);
		//std::sort(m_sources.begin(), m_sources.end(), functors::PrioritySorter());
	}

	PReadStream CFileSystem::findFile(const std::string& strFilePath) const
	{
		PReadStream s;
		std::string total_path	= m_rootPath.string();
		if(total_path != "")
		{
			char last = total_path[total_path.size() - 1];
			if((last != '/') &&
			   (last != '\\'))
			total_path += "/";
		}
		total_path += strFilePath;

		for (Sources::const_iterator it = m_sources.begin(); it != m_sources.end(); ++it)
		{
			s = (*it)->findFile(total_path);
			if (s)
				break;
		}

		return s;
	}

	bool CFileSystem::isExist(const std::string& strFilePath) const
	{
		bool result	= false;
		std::string total_path	= m_rootPath.string() + "/" + strFilePath;

		for (Sources::const_iterator it = m_sources.begin(); it != m_sources.end(); ++it)
		{
			result = (*it)->isExist(total_path);
			if (result)
				break;
		}

		return result;
	}

	ScopePathChange::ScopePathChange(const std::string& strNewPath)
		: m_oldPath(TheFileSystem::Get().getRootDir())
	{
		TheFileSystem::Get().setRootDir(Path(strNewPath));
	}

	ScopePathChange::~ScopePathChange()
	{
		TheFileSystem::Get().setRootDir(m_oldPath);
	}


	ScopePathAdd::ScopePathAdd(const std::string& strNewPath)
		: m_oldPath(TheFileSystem::Get().getRootDir())
	{
		std::string total_path	= m_oldPath.string() + "/" + strNewPath;
		TheFileSystem::Get().setRootDir(Path(total_path));
	}

	ScopePathAdd::~ScopePathAdd()
	{
		TheFileSystem::Get().setRootDir(m_oldPath);
	}
}