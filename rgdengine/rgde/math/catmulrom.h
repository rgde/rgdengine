#pragma once

#include "spline.h"

namespace math
{
    //Catmul-Rom's spline
    template <typename value_t>
    class catm_rom_spline : public spline<value_t>
    {
    public:
        value_t interpolate(float position) const
        {
            if (m_keys.size()<=0)
                return value_t();

            if (m_keys.size()==1)
                return m_keys.front();

            if (position<=0)        return m_keys.front();
            if (position>=length()) return m_keys.back();

            //����� ���������� ��� ������ �������, �� ������ ������� �������� ����������
            int indx = (int)position;
            
			int indx0 = (indx-1 <= 0) ? 0 : indx-1,
                indx1 = indx,
                indx2 = (indx+1 >= (int)m_keys.size()) ? (int)(m_keys.size()-1) : indx+1,
                indx3 = (indx+2 >= (int)m_keys.size()) ? (int)(m_keys.size()-1) : indx+2;
            
			const_iterator i0 = m_keys.begin(),
                           i1 = m_keys.begin(),
                           i2 = m_keys.begin(),
                           i3 = m_keys.begin();

            std::advance(i0,indx0);
            std::advance(i1,indx1);
            std::advance(i2,indx2);
            std::advance(i3,indx3);

            return catmul_rom(*i0,*i1,*i2,*i3, position-indx);
        }

        float length() const
        {
            return m_keys.empty() ? 0.0f : m_keys.size() - 1.0f;
        }

    private:
        value_t catmul_rom (value_t p0, value_t p1, value_t p2, value_t p3, float t) const
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