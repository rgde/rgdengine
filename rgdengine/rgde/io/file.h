#pragma once

#include <rgde/io/path.h>

namespace io
{
	class serialized_object;

	//////////////////////////////////////////////////////////////////////////
	namespace helpers
	{
		std::string get_file_ext(const std::string& filename);
		std::string get_shot_filename(const std::string& filename);
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

		virtual unsigned long size() const = 0;
		virtual unsigned long position() = 0;
		virtual void position(unsigned long pos) = 0;

		virtual ~read_stream(){}
	};

	typedef boost::shared_ptr<read_stream> readstream_ptr;

	class write_stream : public virtual base_stream
	{
	public:
		virtual void write(const byte* buff, unsigned size) = 0;
		virtual ~write_stream(){}
    };
	//////////////////////////////////////////////////////////////////////////
#define SUPPORT_SAVING_SIMPLE_TYPE(type) inline write_stream& operator << (write_stream& wf, type var) \
										 {\
										 	 wf.write((const byte*)&var, sizeof(type));\
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
		typedef std::list<T>::const_iterator iter;

		wf << (unsigned)container.size();

		for (iter it = container.begin(); it != container.end(); ++it)
		{
			wf << *it;
		}
		return wf;
	}

	write_stream& operator << (write_stream& wf, const std::string& str);
	write_stream& operator << (write_stream& wf, const serialized_object& so);
	write_stream& operator << (write_stream& wf, const math::vec3f& vec);
	write_stream& operator << (write_stream& wf, const math::vec4f& vec);
	write_stream& operator << (write_stream& wf, const math::point3f& point);
	write_stream& operator << (write_stream& wf, const math::quatf& quat);
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

	template<typename T>
	inline read_stream& operator >> (read_stream& rf, std::vector<T>& container)
	{
		typedef std::vector<T>::iterator iter;

		unsigned size = 0;
		rf >> size;
		container.resize(size);

		for (iter it = container.begin(); it != container.end(); ++it)
		{
			rf >> *it;
		}
		return rf;
	}

	template<typename T>
	inline read_stream& operator >> (read_stream& rf, std::list<T>& container)
	{
		typedef std::list<T>::iterator iter;

		unsigned size = 0;
		rf >> size;
		container.resize(size);

		for (iter it = container.begin(); it != container.end(); ++it)
		{
			rf >> *it;
		}
		return rf;
	}

	read_stream& operator >> (read_stream& rf, std::string& str);
	read_stream& operator >> (read_stream& rf, serialized_object& so);
	read_stream& operator >> (read_stream& rf, math::vec3f& vec);
	read_stream& operator >> (read_stream& rf, math::vec4f& vec);
	read_stream& operator >> (read_stream& rf, math::point3f& point);
	read_stream& operator >> (read_stream& rf, math::quatf& quat);
	read_stream& operator >> (read_stream& rf, std::wstring& str);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	#pragma warning( disable : 4512 )

	class base_file
	{
	public:
		bool is_open()					const {return m_is_opened;}
		bool is_valid()					const {return m_is_valid;}
		bool has_errors()				const {return m_is_error;}
		const Path& get_path()			const {return m_path;}
		const std::string& name()	const {return m_name;}

		bool open(const std::string& name, Path path = Path());

		virtual ~base_file(){}

	protected:
		virtual bool do_open_file(const std::string& name, const Path& path) = 0;

	protected:
		bool		m_is_opened;
		bool		m_is_valid;
		bool		m_is_error;

		std::string m_name;
		Path		m_path;		
	};

	class write_file : public base_file, public write_stream, boost::noncopyable
	{
	public:
		write_file();
		write_file(const std::string& filename);
		virtual ~write_file();

		virtual bool is_valid() const {return base_file::is_valid();}

		virtual void write(const byte* buff, unsigned size);

	protected:
		virtual bool do_open_file(const std::string& filename, const Path& path);

	protected:
		std::ofstream m_file_stream;
	};

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class read_file 
		: public base_file, public read_stream, boost::noncopyable
	{
	public:
		read_file();
		read_file(const std::string& filename);
		virtual ~read_file();

		virtual bool is_valid() const {return base_file::is_valid();}
		virtual void read(byte* buff, unsigned size);

		virtual unsigned long size() const;
		virtual unsigned long position();
		virtual void position(unsigned long pos);

	protected:		
		virtual bool do_open_file(const std::string& filename, const Path& path);

	protected:
		unsigned long m_size;
		std::ifstream m_file_stream;
	};
	//////////////////////////////////////////////////////////////////////////
}