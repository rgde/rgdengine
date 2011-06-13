//catmulrom_dist.h
#pragma once

#include "spline.h"
#include "catmulrom.h"
#include "interpolators.h"

namespace math
{
    //модификаци€ сплайна  этмула-–ома
    //отличаетс€ тем, что при помощи функции DIST можно задать
    //рассто€ние между двум€ value_t, что используетс€ дл€ равномерного
    //интерполировани€ по сплайну (иными словами, DIST нужен дл€
    //параметризации длинной сплайна)
    template <typename value_t, typename DIST>
    class catm_rom_spline_dist: public spline<value_t>
    {
    public:
        catm_rom_spline_dist(): m_length(0) {}

        value_t interpolate(float position) const
        {
            //вернем что-нить
            if (m_keys.size()<=0)
                return value_t();
            if (m_keys.size()==1)
                return m_keys.front();

            if (position<=0)        return m_keys.front();
            if (position>=length()) return m_keys.back();

            float t = m_position2parameter(position);
            return m_spline.interpolate(t);
        }

        float length() const
        {
            return m_length;
        }

        void apply()
        {
            m_spline.m_keys.swap(catm_rom_spline<value_t>::keys(m_keys));
            m_position2parameter.m_keys.clear();
            m_length = 0;

            if (m_spline.m_keys.size() < 1)
                return;

            catm_rom_spline<value_t>::const_iterator i    =   m_spline.m_keys.begin();
            catm_rom_spline<value_t>::const_iterator last = --m_spline.m_keys.end();
            const float step = 1.f/100.f;
            float t = 0;

            m_position2parameter.add(0,0);
            while (i != last)
            {
                for (float k=0; k<=1; k+=step)
                {
                    value_t v1 = m_spline(t);
                    t+=step;
                    value_t v2 = m_spline(t);

                    DIST Dist;
                    m_length += Dist(v1,v2);
                    m_position2parameter.add(m_length,t);
                }
                ++i;
            }
        }

        float get_param (float position)
        {
            return m_position2parameter(position);
        }

    protected:
        void to_stream(io::write_stream& ws) const
        {
            ws << uint(m_spline.m_keys.size());

            for(catm_rom_spline<value_t>::const_iterator it = m_spline.m_keys.begin(); it != m_spline.m_keys.end(); ++it)
                ws << (*it);
        }

        void from_stream(io::read_stream& rs)
        {
            m_keys.clear();

            uint num;
            rs >> num;
            for(uint i = 0; i < num; ++i)
            {
                value_t value;
                rs >> value;
                m_keys.push_back(value);
            }

            apply();
        }

    private:
        float                    m_length;             //длинна сплайна
        catm_rom_spline<value_t>  m_spline;             //сплайн интерполируемых величин
        FloatLinearInterpolatorf m_position2parameter; //переход от параметризации длинной к параметризации по умолчанию
    };

} //namespace math