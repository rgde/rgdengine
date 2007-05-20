#include "precompiled.h"

#include "io/ioFile.h"
#include "io/ioSerializedObject.h"

namespace io
{
	namespace helpers
	{
		//-----------------------------------------------------------------------------------
		std::string getFileExtension(const std::string &fileName)
		{
			size_t pos	= fileName.rfind(".") + 1;
			return fileName.substr(pos, fileName.size() - pos);
		}

		//-----------------------------------------------------------------------------------
		std::string getFileNameWithoutExtension(const std::string &fileName)
		{
			size_t pos	= fileName.rfind(".");
			return fileName.substr(0, pos);
		}

		////-----------------------------------------------------------------------------------
		//std::string createFullFilePath(const std::string& path)
		//{
		//	std::string fullAddr;
		//	wchar_t buf[MAX_PATH];

		//	GetCurrentDirectory (MAX_PATH, buf);
		//	fullAddr.resize (MAX_PATH);
		//	fullAddr = buf;
		//	fullAddr.append (L"/");
		//	fullAddr.append (path);

		//	return fullAddr;
		//}

		////-------------------------------------------------------------------------------
		//std::string createFullFilePathA (const std::wstring& path)
		//{
		//	return wstr_to_str (createFullFilePath (path));
		//}

		//-------------------------------------------------------------------------------
		std::string wstr_to_str(const std::wstring &wstr)
		{
			return std::string(wstr.begin(), wstr.end());
		}
		//-------------------------------------------------------------------------------
		std::wstring str_to_wstr(const std::string &str)
		{
			return std::wstring(str.begin(), str.end());
		}
	}
	//////////////////////////////////////////////////////////////////////////
	IReadStream & operator >>(IReadStream &rf, std::string &str)
	{
		unsigned size	= 0;
		rf.readbuff((byte *)&size, sizeof(unsigned));
		char *c_str	= new char[size];
		rf.readbuff((byte *)c_str, size * sizeof(char));
		str = std::string(c_str);
		return rf;
	}
	//-----------------------------------------------------------------------------------
	IReadStream & operator >>(IReadStream &rf, ISerializedObject &so)
	{
		so.read(rf);
		return rf;
	}
	//-----------------------------------------------------------------------------------
	IReadStream & operator >>(IReadStream &rf, math::Vec3f &vec)
	{
		rf >> vec[0] >> vec[1] >> vec[2];
		return rf;
	}
	//-----------------------------------------------------------------------------------
	IReadStream & operator >>(IReadStream &rf, math::Vec4f &vec)
	{
		rf >> vec[0] >> vec[1] >> vec[2] >> vec[3];
		return rf;
	}
	//-----------------------------------------------------------------------------------
	IReadStream & operator >>(IReadStream &rf, math::Point3f &point)
	{
		rf >> point[0] >> point[1] >> point[2];
		return rf;
	}
	//-----------------------------------------------------------------------------------
	IReadStream & operator >>(IReadStream &rf, math::Quatf &quat)
	{
		rf >> quat[0] >> quat[1] >> quat[2] >> quat[3];
		return rf;
	}
	//-----------------------------------------------------------------------------------
	IReadStream & operator >>(IReadStream &rf, std::wstring &str)
	{
		unsigned size	= 0;
		rf.readbuff((byte *)&size, sizeof(unsigned));
		wchar_t *c_str	= new wchar_t[size];
		rf.readbuff((byte *)c_str, size * sizeof(wchar_t));
		str = std::wstring(c_str);
		return rf;
	}
	//////////////////////////////////////////////////////////////////////////
	IWriteStream & operator <<(IWriteStream &wf, const std::string &str)
	{
		unsigned size	= (unsigned)str.size();
		size++;
		wf.writebuff((const byte *)&size, sizeof(unsigned));
		wf.writebuff((const byte *)str.c_str(), size * sizeof(char));
		return wf;
	}
	//-----------------------------------------------------------------------------------
	IWriteStream & operator <<(IWriteStream &wf, const ISerializedObject &so)
	{
		so.write(wf);
		return wf;
	}
	//-----------------------------------------------------------------------------------
	IWriteStream & operator <<(IWriteStream &wf, const math::Vec3f &vec)
	{
		wf << vec[0] << vec[1] << vec[2];
		return wf;
	}
	//-----------------------------------------------------------------------------------
	IWriteStream & operator <<(IWriteStream &wf, const math::Vec4f &vec)
	{
		wf << vec[0] << vec[1] << vec[2] << vec[3];
		return wf;
	}
	//-----------------------------------------------------------------------------------
	IWriteStream & operator <<(IWriteStream &wf, const math::Point3f &point)
	{
		wf << point[0] << point[1] << point[2];
		return wf;
	}
	//-----------------------------------------------------------------------------------
	IWriteStream & operator <<(IWriteStream &wf, const math::Quatf &quat)
	{
		wf << quat[0] << quat[1] << quat[2] << quat[3];
		return wf;
	}
	//-----------------------------------------------------------------------------------
	IWriteStream & operator <<(IWriteStream &wf, const std::wstring &str)
	{
		unsigned size	= (unsigned)str.size();
		size++;
		wf.writebuff((const byte *)&size, sizeof(unsigned));
		wf.writebuff((const byte *)str.c_str(), size * sizeof(wchar_t));
		return wf;
	}
	//////////////////////////////////////////////////////////////////////////
	CWriteFileStream::CWriteFileStream()
	{
		m_isOpened = false;
		m_isValid = false;
		m_isError = false;
	}
	//-----------------------------------------------------------------------------------
	CWriteFileStream::CWriteFileStream(const std::string& file_name)
	{
		m_isOpened = open(file_name);
		m_isError = !m_isOpened;
		m_isValid = m_isOpened;
	}
	//-----------------------------------------------------------------------------------
	CWriteFileStream::~CWriteFileStream()
	{
		m_fileStream.flush();
		m_fileStream.close();
	}
	//--------------------------------------------------------------------------------------
	bool CWriteFileStream::do_open_file(const std::string& fullname, const Path& path)
	{
		//std::string fullname = file_name;// + (std::string)path;
		if (m_isOpened)
			m_fileStream.close();
		if (m_isError)
			m_fileStream.clear();
		m_fileStream.open(fullname.c_str(), std::ios_base::binary);

		m_isOpened = m_fileStream.is_open();
		m_isError = !m_fileStream.good();
		m_isValid = m_isOpened;

		return m_isOpened;
	}
	//--------------------------------------------------------------------------------------
	void CWriteFileStream::writebuff(const byte *buff, unsigned size)
	{
		m_fileStream.write((const char*)buff, size);
	}
	//////////////////////////////////////////////////////////////////////////
	CReadFileStream::CReadFileStream()
		: m_size(0)
	{
		m_isOpened = false;
		m_isValid = false;
		m_isError = false;
	}
	//-----------------------------------------------------------------------------------
	CReadFileStream::CReadFileStream(const std::string& file_name)
		: m_size(0)
	{
		m_isOpened = false;
		m_isError = false;
		m_isOpened = open(file_name);
		m_isError = !m_isOpened;
		m_isValid = m_isOpened;
	}
	//-----------------------------------------------------------------------------------
	CReadFileStream::~CReadFileStream()
	{
		m_fileStream.close();
	}
	//-----------------------------------------------------------------------------------
	void CReadFileStream::readbuff(byte *buff, unsigned size)
	{
		m_fileStream.read((char*)buff, size);
	}
	//-----------------------------------------------------------------------------------
	bool CReadFileStream::do_open_file(const std::string& fullname, const Path& path)
	{
		//std::string fullname= file_name;// + (std::string)path;
		if (m_isOpened)
			m_fileStream.close();
		if (m_isError)
			m_fileStream.clear();
		m_fileStream.open(fullname.c_str(), std::ios_base::binary);

		m_isOpened = m_fileStream.is_open();
		m_isError = !m_fileStream.good();
		m_isValid = m_isOpened;

		if (m_isValid)
		{
			m_fileStream.seekg(0, std::ios_base::end);
			m_size = m_fileStream.tellg();
			m_fileStream.seekg(0, std::ios_base::beg);
		}

		return m_isOpened;
	}
	//-----------------------------------------------------------------------------------
	unsigned long CReadFileStream::getSize() const
	{
		return m_size;
	}
	//-----------------------------------------------------------------------------------
	unsigned long CReadFileStream::getPos()
	{
		return m_fileStream.tellg();;
	}
	//-----------------------------------------------------------------------------------
	void CReadFileStream::setPos(unsigned long nPos)
	{
		m_fileStream.seekg(nPos, std::ios_base::beg);
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	bool IFile::open(const std::string& name, Path path)
	{
		return do_open_file(name, path);
	}
}