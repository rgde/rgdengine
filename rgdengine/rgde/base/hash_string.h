#pragma once

namespace base
{
	class hash_string
	{
	public:
		union hash_id
		{
			unsigned char raw_uchar[20];

			union{
				unsigned int raw_uint[5];
				struct{
					unsigned i1, i2, i3, i4, i5;
				};
			};

			void operator=(const hash_id& _hid);
			bool operator==(const hash_id& _hid) const;
		};

		explicit hash_string(const char* _c);
		explicit hash_string(const char* _c, size_t _n);
		explicit hash_string(const std::string& _s);

		hash_string(const hash_string& _hs);

		operator const std::string&() const {return m_string;}

		const hash_id& hash() const {return m_hash_value;}

	protected:
		void calc_hash();

	protected:
		std::string m_string;
		hash_id  m_hash_value;
	};
}