// табличный рандом генератов, для использования в частицах
// позволяет ускорить обсчет генерации и изменения состояния
// частиц.									Korak
#pragma once

#include <stdlib.h>	// for int rand();

namespace math
{
	// размер должен быть степенью 2
	template<unsigned int TableSize>
	class TTableUnitRandom
	{		
	public:
		TTableUnitRandom() : m_nCurIndex(0)
		{
			if(!ms_bIsTableInited)
				for (unsigned i = 0; i < TableSize; ++i)
					ms_vRndTable[i] = rand()/(float)RAND_MAX;

			ms_bIsTableInited = true;
		}

		float operator()()
		{
			m_nCurIndex = (m_nCurIndex + 1) & (TableSize - 1);
			return ms_vRndTable[m_nCurIndex];
		}

		void	 setSeed(unsigned seed) {m_nCurIndex = seed;}

	protected:
		static float	ms_vRndTable[TableSize];
		static bool		ms_bIsTableInited;

		int				m_nCurIndex;
	};

	template<unsigned int TableSize>
	bool TTableUnitRandom<TableSize>::ms_bIsTableInited = false;

	template<unsigned int TableSize>
	float TTableUnitRandom<TableSize>::ms_vRndTable[TableSize];

	typedef TTableUnitRandom<2048> UnitRandom2k;
}