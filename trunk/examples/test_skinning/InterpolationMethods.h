/*

@author PC
*/
#pragma once

#include "Interpolator.h"

template <class TValue, class TPosition>
class LinearInterpolationMethod : public TInterpolationMethod<TValue, TPosition>
{
public:

	LinearInterpolationMethod(const Keys& keys) : TInterpolationMethod<TValue, TPosition>(keys) {}

	TValue interpolate(const TPosition& position) const
	{
		KeysConstIter lessEq = getLessOrEqualKey(position);
		KeysConstIter grEq   = getGreaterOrEqualKey(position);

		if(lessEq == grEq)
			return lessEq->second.getValue();
		else
			if(m_keys.end() == lessEq)
				return grEq->second.getValue();
			else
				if(m_keys.end() == grEq)
					return lessEq->second.getValue();
				else
				{
					TPosition dLessGr = grEq->first - lessEq->first;
					TPosition dLessPos = position - lessEq->first;
					TValue dValue = grEq->second.getValue() - lessEq->second.getValue();
					return dValue*(dLessPos/dLessGr) + lessEq->second.getValue();
				}
	}

};