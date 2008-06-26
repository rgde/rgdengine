#pragma once

#include <rgde/io/path.h>

namespace io
{
	class ISerializedObject;

	//////////////////////////////////////////////////////////////////////////
	namespace helpers
	{
		std::string getFileExtension(const std::string& fileName);
		std::string getFileNameWithoutExtension(const std::string& fileName);
		//std::string createFullFilePath(const std::string& path);
		//std::string createFullFilePathA (const std::string& path);
		std::string wstr_to_str (const std::wstring& wstr);
		std::wstring str_to_wstr (const std::string& str);
	}
	//////////////////////////////////////////////////////////////////////////

	class IStream
	{
	public:
		virtual ~IStream(){}

		virtual bool isValid() const = 0;
	};

	typedef boost::shared_ptr<IStream> PStream;

	class IReadStream : public virtual IStream
	{
	public:
		virtual void readbuff(byte* buff, unsigned size) = 0;

		virtual unsigned long getSize() const = 0;
		virtual unsigned long getPos() = 0;
		virtual void setPos(unsigned long nPos) = 0;

		virtual ~IReadStream(){}
	};

	typedef boost::shared_ptr<IReadStream> PReadStream;

	class IWriteStream : public virtual IStream
	{
	public:
		virtual void writebuff(const byte* buff, unsigned size) = 0;
		virtual ~IWriteStream(){}
    };
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
#define SUPPORT_SAVING_SIMPLE_TYPE(type) inline IWriteStream& operator << (IWriteStream& wf, type var) \
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
	inline IWriteStream& operator << (IWriteStream& wf, const std::vector<T>& container)
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
	inline IWriteStream& operator << (IWriteStream& wf, const std::list<T>& container)
	{
		typedef std::list<T>::const_iterator contIter;

		wf << (unsigned)container.size();

		for (contIter it = container.begin(); it != container.end(); ++it)
		{
			wf << *it;
		}
		return wf;
	}

	IWriteStream& operator << (IWriteStream& wf, const std::string& str);
	IWriteStream& operator << (IWriteStream& wf, const ISerializedObject& so);
	IWriteStream& operator << (IWriteStream& wf, const math::Vec3f& vec);
	IWriteStream& operator << (IWriteStream& wf, const math::Vec4f& vec);
	IWriteStream& operator << (IWriteStream& wf, const math::Point3f& point);
	IWriteStream& operator << (IWriteStream& wf, const math::Quatf& quat);
	IWriteStream& operator << (IWriteStream& wf, const std::wstring& str);


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define SUPPORT_READING_SIMPLE_TYPE(type) inline IReadStream& operator >> (IReadStream& rf, type& var) \
											{\
												rf.readbuff((byte*)&var, sizeof(type));\
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
	inline IReadStream& operator >> (IReadStream& rf, std::vector<T>& container)
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
	inline IReadStream& operator >> (IReadStream& rf, std::list<T>& container)
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

	IReadStream& operator >> (IReadStream& rf, std::string& str);
	IReadStream& operator >> (IReadStream& rf, ISerializedObject& so);
	IReadStream& operator >> (IReadStream& rf, math::Vec3f& vec);
	IReadStream& operator >> (IReadStream& rf, math::Vec4f& vec);
	IReadStream& operator >> (IReadStream& rf, math::Point3f& point);
	IReadStream& operator >> (IReadStream& rf, math::Quatf& quat);
	IReadStream& operator >> (IReadStream& rf, std::wstring& str);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	#pragma warning( disable : 4512 )

	class IFile
	{
	public:
		bool isOpened()					const {return m_isOpened;}
		bool isValid()					const {return m_isValid;}
		bool isHasError()				const {return m_isError;}
		const Path& GetPath()			const {return m_path;}
		const std::string& getName()	const {return m_name;}

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

	class CWriteFileStream : public IFile, public IWriteStream, boost::noncopyable
	{
	public:
		CWriteFileStream();
		CWriteFileStream(const std::string& file_name);
		virtual ~CWriteFileStream();

		virtual bool isValid() const {return IFile::isValid();}

		virtual void writebuff(const byte* buff, unsigned size);

	protected:
		virtual bool do_open_file(const std::string& file_name, const Path& path);

	protected:
		std::ofstream m_fileStream;
	};

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class CReadFileStream 
		: public IFile, public IReadStream, boost::noncopyable
	{
	public:
		CReadFileStream();
		CReadFileStream(const std::string& file_name);
		virtual ~CReadFileStream();

		virtual bool isValid() const {return IFile::isValid();}
		virtual void readbuff(byte* buff, unsigned size);

		virtual unsigned long getSize() const;
		virtual unsigned long getPos();
		virtual void setPos(unsigned long nPos);

	protected:		
		virtual bool do_open_file(const std::string& file_name, const Path& path);

	protected:
		unsigned long m_size;
		std::ifstream m_fileStream;
	};
	//////////////////////////////////////////////////////////////////////////
}