#pragma once

#pragma warning( disable : 4201 )

namespace std_ext
{
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

	class hash_string : public std::string
	{
	public:		
		hash_string();
		hash_string(const char* _c);
		hash_string(char* _c);
		hash_string(char* _c, size_t _n);
		hash_string(const std::string& _s);
		hash_string(const hash_string& _hs);

		const hash_id& hash();
	protected:
		void calc_hash();
	protected:
		bool m_isHashCalculated;
		hash_id  m_hash_value;
	};
}