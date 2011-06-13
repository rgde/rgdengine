#include "precompiled.h"

#include <rgde/io/file.h>
#include <rgde/io/serialized_object.h>

namespace io
{
	namespace helpers
	{
		//-----------------------------------------------------------------------------------
		std::string get_file_ext(const std::string &filename)
		{
			size_t pos	= filename.rfind(".") + 1;
			return filename.substr(pos, filename.size() - pos);
		}

		//-----------------------------------------------------------------------------------
		std::string get_shot_filename(const std::string &filename)
		{
			size_t pos	= filename.rfind(".");
			return filename.substr(0, pos);
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
	read_stream & operator >>(read_stream &rf, std::string &str)
	{
		unsigned size	= 0;
		rf.read((byte *)&size, sizeof(unsigned));
		char *c_str	= new char[size];
		rf.read((byte *)c_str, size * sizeof(char));
		str = std::string(c_str);
		return rf;
	}
	//-----------------------------------------------------------------------------------
	read_stream & operator >>(read_stream &rf, serialized_object &so)
	{
		so.read(rf);
		return rf;
	}
	//-----------------------------------------------------------------------------------
	read_stream & operator >>(read_stream &rf, math::vec3f &vec)
	{
		rf >> vec[0] >> vec[1] >> vec[2];
		return rf;
	}
	//-----------------------------------------------------------------------------------
	read_stream & operator >>(read_stream &rf, math::vec4f &vec)
	{
		rf >> vec[0] >> vec[1] >> vec[2] >> vec[3];
		return rf;
	}
	//-----------------------------------------------------------------------------------
	read_stream & operator >>(read_stream &rf, math::point3f &point)
	{
		rf >> point[0] >> point[1] >> point[2];
		return rf;
	}
	//-----------------------------------------------------------------------------------
	read_stream & operator >>(read_stream &rf, math::quatf &quat)
	{
		rf >> quat[0] >> quat[1] >> quat[2] >> quat[3];
		return rf;
	}
	//-----------------------------------------------------------------------------------
	read_stream & operator >>(read_stream &rf, std::wstring &str)
	{
		unsigned size	= 0;
		rf.read((byte *)&size, sizeof(unsigned));
		wchar_t *c_str	= new wchar_t[size];
		rf.read((byte *)c_str, size * sizeof(wchar_t));
		str = std::wstring(c_str);
		return rf;
	}
	//////////////////////////////////////////////////////////////////////////
	write_stream & operator <<(write_stream &wf, const std::string &str)
	{
		unsigned size	= (unsigned)str.size();
		size++;
		wf.write((const byte *)&size, sizeof(unsigned));
		wf.write((const byte *)str.c_str(), size * sizeof(char));
		return wf;
	}
	//-----------------------------------------------------------------------------------
	write_stream & operator <<(write_stream &wf, const serialized_object &so)
	{
		so.write(wf);
		return wf;
	}
	//-----------------------------------------------------------------------------------
	write_stream & operator <<(write_stream &wf, const math::vec3f &vec)
	{
		wf << vec[0] << vec[1] << vec[2];
		return wf;
	}
	//-----------------------------------------------------------------------------------
	write_stream & operator <<(write_stream &wf, const math::vec4f &vec)
	{
		wf << vec[0] << vec[1] << vec[2] << vec[3];
		return wf;
	}
	//-----------------------------------------------------------------------------------
	write_stream & operator <<(write_stream &wf, const math::point3f &point)
	{
		wf << point[0] << point[1] << point[2];
		return wf;
	}
	//-----------------------------------------------------------------------------------
	write_stream & operator <<(write_stream &wf, const math::quatf &quat)
	{
		wf << quat[0] << quat[1] << quat[2] << quat[3];
		return wf;
	}
	//-----------------------------------------------------------------------------------
	write_stream & operator <<(write_stream &wf, const std::wstring &str)
	{
		unsigned size	= (unsigned)str.size();
		size++;
		wf.write((const byte *)&size, sizeof(unsigned));
		wf.write((const byte *)str.c_str(), size * sizeof(wchar_t));
		return wf;
	}
	//////////////////////////////////////////////////////////////////////////
	write_file::write_file()
	{
		m_is_opened = false;
		m_is_valid = false;
		m_is_error = false;
	}
	//-----------------------------------------------------------------------------------
	write_file::write_file(const std::string& filename)
	{
		m_is_opened = open(filename);
		m_is_error = !m_is_opened;
		m_is_valid = m_is_opened;
	}
	//-----------------------------------------------------------------------------------
	write_file::~write_file()
	{
		m_file_stream.flush();
		m_file_stream.close();
	}
	//--------------------------------------------------------------------------------------
	bool write_file::do_open_file(const std::string& fullname, const Path& path)
	{
		//std::string fullname = filename;// + (std::string)path;
		if (m_is_opened)
			m_file_stream.close();
		if (m_is_error)
			m_file_stream.clear();
		m_file_stream.open(fullname.c_str(), std::ios_base::binary);

		m_is_opened = m_file_stream.is_open();
		m_is_error = !m_file_stream.good();
		m_is_valid = m_is_opened;

		return m_is_opened;
	}
	//--------------------------------------------------------------------------------------
	void write_file::write(const byte *buff, unsigned size)
	{
		m_file_stream.write((const char*)buff, size);
	}
	//////////////////////////////////////////////////////////////////////////
	read_file::read_file()
		: m_size(0)
	{
		m_is_opened = false;
		m_is_valid = false;
		m_is_error = false;
	}
	//-----------------------------------------------------------------------------------
	read_file::read_file(const std::string& filename)
		: m_size(0)
	{
		m_is_opened = false;
		m_is_error = false;
		m_is_opened = open(filename);
		m_is_error = !m_is_opened;
		m_is_valid = m_is_opened;
	}
	//-----------------------------------------------------------------------------------
	read_file::~read_file()
	{
		m_file_stream.close();
	}
	//-----------------------------------------------------------------------------------
	void read_file::read(byte *buff, unsigned size)
	{
		m_file_stream.read((char*)buff, size);
	}
	//-----------------------------------------------------------------------------------
	bool read_file::do_open_file(const std::string& fullname, const Path& path)
	{
		//std::string fullname= filename;// + (std::string)path;
		if (m_is_opened)
			m_file_stream.close();
		if (m_is_error)
			m_file_stream.clear();
		m_file_stream.open(fullname.c_str(), std::ios_base::binary);

		m_is_opened = m_file_stream.is_open();
		m_is_error = !m_file_stream.good();
		m_is_valid = m_is_opened;

		if (m_is_valid)
		{
			m_file_stream.seekg(0, std::ios_base::end);
			m_size = m_file_stream.tellg();
			m_file_stream.seekg(0, std::ios_base::beg);
		}

		return m_is_opened;
	}
	//-----------------------------------------------------------------------------------
	unsigned long read_file::size() const
	{
		return m_size;
	}
	//-----------------------------------------------------------------------------------
	unsigned long read_file::position()
	{
		return m_file_stream.tellg();;
	}
	//-----------------------------------------------------------------------------------
	void read_file::position(unsigned long pos)
	{
		m_file_stream.seekg(pos, std::ios_base::beg);
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	bool base_file::open(const std::string& name, Path path)
	{
		return do_open_file(name, path);
	}
}