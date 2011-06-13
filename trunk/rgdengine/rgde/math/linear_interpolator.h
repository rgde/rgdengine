#pragma once

#include "interpolator.h"

namespace math
{
    template <typename value_t, typename param_t>
    class linear_interpolator: public base_interpolator<value_t, param_t>
    {
    public:
        value_t interpolate(param_t position) const
        {
            const_iterator lessEq = getLessOrEqualIterator(position);
            const_iterator grEq   = getGreaterOrEqualIterator(position);

            if(lessEq == m_keys.end() || grEq == m_keys.end())
            {
                if (lessEq != m_keys.end()) return lessEq->second;
                if (grEq != m_keys.end()) return grEq->second;
				assert(false && "Invalid internal data");
                return value_t(); 
            }
            if(lessEq == grEq)
                return lessEq->second;

            param_t distance = grEq->first - lessEq->first;
            param_t t = position - lessEq->first;
            value_t v = grEq->second - lessEq->second;
            return v*(t/distance) + lessEq->second;
        }
    };
}