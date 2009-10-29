#pragma once

#include <rgde/io/path.h>

namespace io
{
	class serialized_object;

	//////////////////////////////////////////////////////////////////////////
	namespace helpers
	{
		std::string get_file_ext(const std::string& file_name);
		std::string get_shot_filename(const std::string& file_name);
		//std::string createFullFilePath(const std::string& path);
		//std::string createFullFilePathA (const std::string& path);
		std::string wstr_to_str (const std::wstring& wstr);
		std::wstring str_to_wstr (const std::string& str);
	}
	//////////////////////////////////////////////////////////////////////////

	struct base_stream
	{
		virtual ~base_stream(){}
		virtual bool is_valid() const = 0;
	};

	typedef boost::shared_ptr<base_stream> stream_ptr;

	class read_stream : public base_stream
	{
	public:
		virtual void read(byte* buff, unsigned size) = 0;

		virtual unsigned long get_size() const = 0;
		virtual unsigned long get_pos() = 0;
		virtual void set_pos(unsigned long pos) = 0;

		virtual ~read_stream(){}
	};

	typedef boost::shared_ptr<read_stream> readstream_ptr;

	class write_stream : public virtual base_stream
	{
	public:
		virtual void writebuff(const byte* buff, unsigned size) = 0;
		virtual ~write_stream(){}
    };
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
#define SUPPORT_SAVING_SIMPLE_TYPE(type) inline write_stream& operator << (write_stream& wf, type var) \
										 {\
										 	 wf.writebuff((const byte*)&var, sizeof(type));\
											 return wf;\
										 }

	SUPPORT_SAVING_SIMPLE_TYPE(bool)
	SUPPORT_SAVING_SIMPLE_TYPE(int)
	SUPPORT_SAVING_SIMPLE_TYPE(unsigned)
	SUPPORT_SAVING_SIMPLE_TYPE(float)
	SUPPORT_SAVING_SIMPLE_TYPE(double)
	SUPPORT_SAVING_SIMPLE_TYPE(char)
	SUPPORT_SAVING_SIMPLE_TYPE(unsigned char)
	SUPPORT_SAVING_SIMPLE_TYPE(unsigned long)
//	SUPPORT_SAVING_SIMPLE_TYPE(size_t)

	template<typename T>
	inline write_stream& operator << (write_stream& wf, const std::vector<T>& container)
	{
		typedef std::vector<T>::const_iterator Iter;

		wf << (unsigned)container.size();

		for (Iter it = container.begin(); it != container.end(); ++it)
		{
			wf << *it;
		}
		return wf;
	}

	template<typename T>
	inline write_stream& operator << (write_stream& wf, const std::list<T>& container)
	{
		typedef std::list<T>::const_iterator contIter;

		wf << (unsigned)container.size();

		for (contIter it = container.begin(); it != container.end(); ++it)
		{
			wf << *it;
		}
		return wf;
	}

	write_stream& operator << (write_stream& wf, const std::string& str);
	write_stream& operator << (write_stream& wf, const serialized_object& so);
	write_stream& operator << (write_stream& wf, const math::Vec3f& vec);
	write_stream& operator << (write_stream& wf, const math::Vec4f& vec);
	write_stream& operator << (write_stream& wf, const math::Point3f& point);
	write_stream& operator << (write_stream& wf, const math::Quatf& quat);
	write_stream& operator << (write_stream& wf, const std::wstring& str);


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define SUPPORT_READING_SIMPLE_TYPE(type) inline read_stream& operator >> (read_stream& rf, type& var) \
											{\
												rf.read((byte*)&var, sizeof(type));\
												return rf;\
											}

	SUPPORT_READING_SIMPLE_TYPE(bool)
	SUPPORT_READING_SIMPLE_TYPE(int)
	SUPPORT_READING_SIMPLE_TYPE(unsigned)
	SUPPORT_READING_SIMPLE_TYPE(float)
	SUPPORT_READING_SIMPLE_TYPE(double)
	SUPPORT_READING_SIMPLE_TYPE(char)
	SUPPORT_READING_SIMPLE_TYPE(unsigned char)
	SUPPORT_READING_SIMPLE_TYPE(unsigned long)
	//	SUPPORT_SAVING_SIMPLE_TYPE(size_t)

	template<typename T>
	inline read_stream& operator >> (read_stream& rf, std::vector<T>& container)
	{
		typedef std::vector<T>::iterator contIter;

		unsigned size = 0;
		rf >> size;
		container.resize(size);

		for (contIter it = container.begin(); it != container.end(); ++it)
		{
			rf >> *it;
		}
		return rf;
	}

	template<typename T>
	inline read_stream& operator >> (read_stream& rf, std::list<T>& container)
	{
		typedef std::list<T>::iterator contIter;

		unsigned size = 0;
		rf >> size;
		container.resize(size);

		for (contIter it = container.begin(); it != container.end(); ++it)
		{
			rf >> *it;
		}
		return rf;
	}

	read_stream& operator >> (read_stream& rf, std::string& str);
	read_stream& operator >> (read_stream& rf, serialized_object& so);
	read_stream& operator >> (read_stream& rf, math::Vec3f& vec);
	read_stream& operator >> (read_stream& rf, math::Vec4f& vec);
	read_stream& operator >> (read_stream& rf, math::Point3f& point);
	read_stream& operator >> (read_stream& rf, math::Quatf& quat);
	read_stream& operator >> (read_stream& rf, std::wstring& str);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	#pragma warning( disable : 4512 )

	class IFile
	{
	public:
		bool isOpened()					const {return m_isOpened;}
		bool is_valid()					const {return m_isValid;}
		bool isHasError()				const {return m_isError;}
		const Path& GetPath()			const {return m_path;}
		const std::string& get_name()	const {return m_name;}

		bool open(const std::string& name, Path path = Path());

		virtual ~IFile(){}

	protected:
		virtual bool do_open_file(const std::string& name, const Path& path) = 0;

	protected:
		bool		m_isOpened;
		bool		m_isValid;
		bool		m_isError;

		std::string m_name;
		Path		m_path;		
	};

	class CWriteFileStream : public IFile, public write_stream, boost::noncopyable
	{
	public:
		CWriteFileStream();
		CWriteFileStream(const std::string& file_name);
		virtual ~CWriteFileStream();

		virtual bool is_valid() const {return IFile::is_valid();}

		virtual void writebuff(const byte* buff, unsigned size);

	protected:
		virtual bool do_open_file(const std::string& file_name, const Path& path);

	protected:
		std::ofstream m_fileStream;
	};

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class read_file_stream 
		: public IFile, public read_stream, boost::noncopyable
	{
	public:
		read_file_stream();
		read_file_stream(const std::string& file_name);
		virtual ~read_file_stream();

		virtual bool is_valid() const {return IFile::is_valid();}
		virtual void read(byte* buff, unsigned size);

		virtual unsigned long get_size() const;
		virtual unsigned long get_pos();
		virtual void set_pos(unsigned long pos);

	protected:		
		virtual bool do_open_file(const std::string& file_name, const Path& path);

	protected:
		unsigned long m_size;
		std::ifstream m_fileStream;
	};
	//////////////////////////////////////////////////////////////////////////
}