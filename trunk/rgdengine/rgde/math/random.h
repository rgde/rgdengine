// табличный рандом генератов, для использования в частицах
// позволяет ускорить обсчет генерации и изменения состояния
// частиц.									Korak
#pragma once

#include <stdlib.h>	// for int rand();

namespace math
{
	// размер должен быть степенью 2
	template<unsigned int table_size>
	class unit_rand_table
	{		
	public:
		unit_rand_table() : m_nCurIndex(0)
		{
			if(!ms_bIsTableInited)
				for (unsigned i = 0; i < table_size; ++i)
					ms_vRndTable[i] = rand()/(float)RAND_MAX;

			ms_bIsTableInited = true;
		}

		float operator()()
		{
			m_nCurIndex = (m_nCurIndex + 1) & (table_size - 1);
			return ms_vRndTable[m_nCurIndex];
		}

		void	 seed(unsigned seed) {m_nCurIndex = seed;}

	protected:
		static float	ms_vRndTable[table_size];
		static bool		ms_bIsTableInited;

		int				m_nCurIndex;
	};

	template<unsigned int table_size>
	bool unit_rand_table<table_size>::ms_bIsTableInited = false;

	template<unsigned int table_size>
	float unit_rand_table<table_size>::ms_vRndTable[table_size];

	typedef unit_rand_table<2048> unit_rand_2k;
}