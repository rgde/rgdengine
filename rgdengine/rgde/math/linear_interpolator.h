//linear_interpolator.h
#pragma once

#include "interpolator.h"

namespace math
{

    //линейный интерполятор
    template <typename VALUE, typename POSITION>
    class TLinearInterpolator: public interpolator_ex<VALUE, POSITION>
    {
    public:
        VALUE interpolate(POSITION position) const
        {
            const_iterator lessEq = getLessOrEqualIterator(position);
            const_iterator grEq   = getGreaterOrEqualIterator(position);

            if(lessEq == m_values.end() || grEq == m_values.end())
            {
                if (lessEq != m_values.end()) return lessEq->second;
                if (grEq != m_values.end()) return grEq->second;
                return VALUE(); //ахтунг! сюда вообще можно ассерт засунуть или брекпоинт [Dub]
            }
            if(lessEq == grEq)
                return lessEq->second;

            POSITION distance = grEq->first - lessEq->first;
            POSITION t = position - lessEq->first;
            VALUE dValue = grEq->second - lessEq->second;
            return dValue*(t/distance) + lessEq->second;
        }
    };

}