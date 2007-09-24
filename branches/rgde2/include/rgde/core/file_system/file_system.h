#pragma once

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

// основные концепции:
// есть файловая система
// есть источники файлов
// они имеют приоритеты
// каждый источник файлов может обладать собственным способом поиска файла (проверки его существования)
// соотв. для каждого источника файлов существуют свои методы чтения и записи данных. (источник является фабрикой)
// обобщая:
// всегда есть хотя бы 1 источник файлов - файловая система диска с которого было запущено приложение
// дефолтный источник может не иметь возможности записи (конфиги хранить в пользовательской дирректории!!! или в реестре, через систему опшенсов!!!)
// опционально будут написаны обвязки для сетевого файлового источника, архивного, и асинхронного.
// так же источник файлов который проверит текущий модуль на наличие файлового архива, если такого не будет обнаружено 
// он будет пытаться открыть архив "data.pak"


namespace rgde
{
	typedef std::vector<char> buffer;
	typedef unsigned int uint;

	namespace file_system
	{
		namespace helpers
		{
			std::wstring get_file_name(const std::wstring& full_name);
			std::wstring get_file_ext(const std::wstring& full_name);
			std::wstring get_file_path(const std::wstring& full_name);

			bool is_file(const std::wstring& full_name) {return get_file_ext(full_name).length() != 0;}
			bool is_directory(const std::wstring& full_name) {return !is_file(full_name);}
		}

		struct IStream
		{
			virtual ~IStream() {}

			virtual uint get_position() const = 0;
			virtual bool set_position() = 0;
			virtual uint get_size() const = 0;
		};

		typedef boost::shared_ptr<IStream> PStream;

		struct IWriteStream : public IStream
		{
			virtual bool write(const buffer& data, uint size) = 0;
		};

		typedef boost::shared_ptr<IWriteStream> PWriteStream;

		struct IReadStream : public IStream
		{
			virtual bool read(buffer& data, uint size) = 0;
		};

		typedef boost::shared_ptr<IReadStream> PReadStream;

		class IFile
		{
		public:
			virtual ~IFile(){}

			bool is_read_only() const {return m_read_only;}
			const std::wstring& get_full_name() const {return m_full_name;}

			virtual PReadStream  open_read_stream() const = 0;

			/// returns empty pointer if READ_ONLY.
			virtual PWriteStream open_write_stream() = 0;

		protected:
			bool		 m_read_only;
			std::wstring m_full_name;
		};

		typedef boost::shared_ptr<IFile> PFile;

		struct IFileSource
		{
			virtual ~IFileSource() {}

			/// returns empty shared_ptr if file not found 
			virtual PFile find(const std::wstring full_name) const = 0;

			virtual bool is_support_enumeration() const {return false;}
			virtual void enumerate(std::vector<PFile>& files, const std::wstring& path) const {}
		};

		typedef boost::shared_ptr<IFileSource> PFileSource;

		class FileSystem : boost::noncopyable
		{
		public:
			FileSystem();
			~FileSystem();

			void add_source(PFileSource file_source, uint priority = 0);

			/// returns empty shared_ptr if file not found 
			PFile find(const std::wstring full_name) const;

		protected:
			typedef std::vector<PFileSource> FileSources;
			FileSources m_sources;
		};

	}
}