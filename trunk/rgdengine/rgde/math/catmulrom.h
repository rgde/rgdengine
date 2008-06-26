#pragma once

#include "spline.h"

namespace math
{
    //сплайн  этмула-–ома
    template <typename VALUE>
    class TCatmulRomSpline: public TSpline<VALUE>
    {
    public:
        VALUE interpolate(float position) const
        {
            //вернем что-нить
            if (m_values.size()<=0)
                return VALUE();
            if (m_values.size()==1)
                return m_values.front();

            if (position<=0)        return m_values.front();
            if (position>=length()) return m_values.back();

            //поиск итераторов тех вершин сплайна, на основе которых стро€тс€ вычислени€
            int indx = (int)position;
            int indx0 = (indx-1 <= 0) ? 0 : indx-1,
                indx1 = indx,
                indx2 = (indx+1 >= (int)m_values.size()) ? (int)(m_values.size()-1) : indx+1,
                indx3 = (indx+2 >= (int)m_values.size()) ? (int)(m_values.size()-1) : indx+2;
            const_iterator i0 = m_values.begin(),
                           i1 = m_values.begin(),
                           i2 = m_values.begin(),
                           i3 = m_values.begin();
            std::advance(i0,indx0);
            std::advance(i1,indx1);
            std::advance(i2,indx2);
            std::advance(i3,indx3);

            return catmulRom(*i0,*i1,*i2,*i3, position-indx);
        }

        float length() const
        {
            return m_values.empty() ? 0.0f : m_values.size()-1.0f;
        }

    private:
        VALUE catmulRom (VALUE p0, VALUE p1, VALUE p2, VALUE p3, float t) const
        {
            return        
                (
                    p1*2.f + 
                    (p0*(-1.f) + p2) * t +
                    (p0*( 2.f) + p1*(-5.f) + p2*( 4.f) + p3*(-1.f)) * t*t +
                    (p0*(-1.f) + p1*( 3.f) + p2*(-3.f) + p3) * t*t*t
                ) * 0.5f;
        }
    };
}