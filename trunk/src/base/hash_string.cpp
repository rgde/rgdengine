#include "precompiled.h"
#include "base/hash_string.h"

#include <stdio.h>  // Needed for file access
#include <memory.h> // Needed for memset and memcpy
#include <string.h> // Needed for strcat and strcpy
//////////////////////////////////////////////////////////////////////////

// If you're compiling big endian, just comment out the following line
#define SHA1_LITTLE_ENDIAN

typedef union
{
	unsigned char c[64];
	unsigned long l[16];
} SHA1_WORKSPACE_BLOCK;

class CSHA1
{
public:
	// Two different formats for ReportHash(...)
	enum
	{
		REPORT_HEX = 0,
		REPORT_DIGIT = 1
	};

	// Constructor and Destructor
	CSHA1();
	virtual ~CSHA1();

	void Reset();

	// Update the hash value
	void Update(unsigned char *data, size_t len);

	// Finalize hash and report
	void Final();
	hash_string::hash_id GetHash();
private:
	unsigned long m_state[5];
	unsigned long m_count[2];
	unsigned char m_buffer[64];
	unsigned char m_digest[20];
	// Private SHA-1 transformation
	void Transform(unsigned long state[5], unsigned char buffer[64]);

	// Member variables
	unsigned char m_workspace[64];
	SHA1_WORKSPACE_BLOCK *m_block; // SHA1 pointer to the byte array above
};


//////////////////////////////////////////////////////////////////////////
void hash_string::hash_id::operator=(const hash_string::hash_id& _hid)
{
	i1 = _hid.i1; 
	i2 = _hid.i2; 
	i3 = _hid.i3; 
	i4 = _hid.i4; 
	i5 = _hid.i5;
}

bool hash_string::hash_id::operator==(const hash_string::hash_id& _hid) const
{
	if (i1 != _hid.i1)
		return false;
	else if (i2 != _hid.i2)
		return false;
	else if (i3 != _hid.i3)
		return false;
	else if (i4 != _hid.i4)
		return false;
	else if (i5 != _hid.i5)
		return false;

	return true;
}
//////////////////////////////////////////////////////////////////////////
hash_string::hash_string(const char* _c): m_string(_c)
{
	calc_hash();
}

hash_string::hash_string(const char* _c, size_t _n) : m_string(_c, _n)
{
	calc_hash();
}

hash_string::hash_string(const std::string& _s) : m_string(_s) 
{
	calc_hash();
}

hash_string::hash_string(const hash_string& _hs) : 	
	m_string(_hs.m_string), 
	m_hash_value(_hs.m_hash_value)
{
}

void hash_string::calc_hash()
{
	CSHA1 sha1;

	sha1.Reset();
	sha1.Update((unsigned char *)m_string.c_str(), m_string.size());
	sha1.Final();

	m_hash_value = sha1.GetHash();
}
//////////////////////////////////////////////////////////////////////////
#define MAX_FILE_READ_BUFFER 8000

// Rotate x bits to the left
#ifndef ROL32
#define ROL32(_val32, _nBits) (((_val32)<<(_nBits))|((_val32)>>(32-(_nBits))))
#endif

#ifdef SHA1_LITTLE_ENDIAN
#define SHABLK0(i) (m_block->l[i] = \
	(ROL32(m_block->l[i],24) & 0xFF00FF00) | (ROL32(m_block->l[i],8) & 0x00FF00FF))
#else
#define SHABLK0(i) (m_block->l[i])
#endif

#define SHABLK(i) (m_block->l[i&15] = ROL32(m_block->l[(i+13)&15] ^ m_block->l[(i+8)&15] \
	^ m_block->l[(i+2)&15] ^ m_block->l[i&15],1))

// SHA-1 rounds
#define _R0(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK0(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R1(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R2(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0x6ED9EBA1+ROL32(v,5); w=ROL32(w,30); }
#define _R3(v,w,x,y,z,i) { z+=(((w|x)&y)|(w&x))+SHABLK(i)+0x8F1BBCDC+ROL32(v,5); w=ROL32(w,30); }
#define _R4(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0xCA62C1D6+ROL32(v,5); w=ROL32(w,30); }

CSHA1::CSHA1()
{
	m_block = (SHA1_WORKSPACE_BLOCK *)m_workspace;

	Reset();
}

CSHA1::~CSHA1()
{
	Reset();
}

void CSHA1::Reset()
{
	// SHA1 initialization constants
	m_state[0] = 0x67452301;
	m_state[1] = 0xEFCDAB89;
	m_state[2] = 0x98BADCFE;
	m_state[3] = 0x10325476;
	m_state[4] = 0xC3D2E1F0;

	m_count[0] = 0;
	m_count[1] = 0;
}

void CSHA1::Transform(unsigned long state[5], unsigned char buffer[64])
{
	unsigned long a = 0, b = 0, c = 0, d = 0, e = 0;

	memcpy(m_block, buffer, 64);

	// Copy state[] to working vars
	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];

	// 4 rounds of 20 operations each. Loop unrolled.
	_R0(a,b,c,d,e, 0); _R0(e,a,b,c,d, 1); _R0(d,e,a,b,c, 2); _R0(c,d,e,a,b, 3);
	_R0(b,c,d,e,a, 4); _R0(a,b,c,d,e, 5); _R0(e,a,b,c,d, 6); _R0(d,e,a,b,c, 7);
	_R0(c,d,e,a,b, 8); _R0(b,c,d,e,a, 9); _R0(a,b,c,d,e,10); _R0(e,a,b,c,d,11);
	_R0(d,e,a,b,c,12); _R0(c,d,e,a,b,13); _R0(b,c,d,e,a,14); _R0(a,b,c,d,e,15);
	_R1(e,a,b,c,d,16); _R1(d,e,a,b,c,17); _R1(c,d,e,a,b,18); _R1(b,c,d,e,a,19);
	_R2(a,b,c,d,e,20); _R2(e,a,b,c,d,21); _R2(d,e,a,b,c,22); _R2(c,d,e,a,b,23);
	_R2(b,c,d,e,a,24); _R2(a,b,c,d,e,25); _R2(e,a,b,c,d,26); _R2(d,e,a,b,c,27);
	_R2(c,d,e,a,b,28); _R2(b,c,d,e,a,29); _R2(a,b,c,d,e,30); _R2(e,a,b,c,d,31);
	_R2(d,e,a,b,c,32); _R2(c,d,e,a,b,33); _R2(b,c,d,e,a,34); _R2(a,b,c,d,e,35);
	_R2(e,a,b,c,d,36); _R2(d,e,a,b,c,37); _R2(c,d,e,a,b,38); _R2(b,c,d,e,a,39);
	_R3(a,b,c,d,e,40); _R3(e,a,b,c,d,41); _R3(d,e,a,b,c,42); _R3(c,d,e,a,b,43);
	_R3(b,c,d,e,a,44); _R3(a,b,c,d,e,45); _R3(e,a,b,c,d,46); _R3(d,e,a,b,c,47);
	_R3(c,d,e,a,b,48); _R3(b,c,d,e,a,49); _R3(a,b,c,d,e,50); _R3(e,a,b,c,d,51);
	_R3(d,e,a,b,c,52); _R3(c,d,e,a,b,53); _R3(b,c,d,e,a,54); _R3(a,b,c,d,e,55);
	_R3(e,a,b,c,d,56); _R3(d,e,a,b,c,57); _R3(c,d,e,a,b,58); _R3(b,c,d,e,a,59);
	_R4(a,b,c,d,e,60); _R4(e,a,b,c,d,61); _R4(d,e,a,b,c,62); _R4(c,d,e,a,b,63);
	_R4(b,c,d,e,a,64); _R4(a,b,c,d,e,65); _R4(e,a,b,c,d,66); _R4(d,e,a,b,c,67);
	_R4(c,d,e,a,b,68); _R4(b,c,d,e,a,69); _R4(a,b,c,d,e,70); _R4(e,a,b,c,d,71);
	_R4(d,e,a,b,c,72); _R4(c,d,e,a,b,73); _R4(b,c,d,e,a,74); _R4(a,b,c,d,e,75);
	_R4(e,a,b,c,d,76); _R4(d,e,a,b,c,77); _R4(c,d,e,a,b,78); _R4(b,c,d,e,a,79);

	// Add the working vars back into state[]
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;

	// Wipe variables
	a = b = c = d = e = 0;
}

// Use this function to hash in binary data and strings
void CSHA1::Update(unsigned char *data, size_t _len)
{
	unsigned long i = 0, j;
	unsigned len = (unsigned)_len;

	j = (m_count[0] >> 3) & 63;

	if((m_count[0] += len << 3) < (len << 3)) m_count[1]++;

	m_count[1] += (len >> 29);

	if((j + len) > 63)
	{
		memcpy(&m_buffer[j], data, (i = 64 - j));
		Transform(m_state, m_buffer);

		for (; i+63 < len; i += 64)
			Transform(m_state, &data[i]);

		j = 0;
	}
	else i = 0;

	memcpy(&m_buffer[j], &data[i], len - i);
}



void CSHA1::Final()
{
	unsigned long i = 0, j = 0;
	unsigned char finalcount[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	for (i = 0; i < 8; i++)
		finalcount[i] = (unsigned char)((m_count[(i >= 4 ? 0 : 1)]
		>> ((3 - (i & 3)) * 8) ) & 255); // Endian independent

		Update((unsigned char *)"\200", 1);

		while ((m_count[0] & 504) != 448)
			Update((unsigned char *)"\0", 1);

		Update(finalcount, 8); // Cause a SHA1Transform()

		for (i = 0; i < 20; i++)
		{
			m_digest[i] = (unsigned char)((m_state[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
		}

		// Wipe variables for security reasons
		i = 0; j = 0;
		memset(m_buffer, 0, 64);
		memset(m_state, 0, 20);
		memset(m_count, 0, 8);
		memset(finalcount, 0, 8);

		Transform(m_state, m_buffer);
}

// Get the raw message digest
hash_string::hash_id CSHA1::GetHash()
{
	hash_string::hash_id v;
	memcpy(v.raw_uchar, m_digest, 20);
	return v;
}