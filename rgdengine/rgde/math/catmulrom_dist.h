//catmulrom_dist.h
#pragma once

#include "spline.h"
#include "catmulrom.h"
#include "interpolators.h"

namespace math
{

    //модификаци€ сплайна  этмула-–ома
    //отличаетс€ тем, что при помощи функции DIST можно задать
    //рассто€ние между двум€ VALUE, что используетс€ дл€ равномерного
    //интерполировани€ по сплайну (иными словами, DIST нужен дл€
    //параметризации длинной сплайна)
    template <typename VALUE, typename DIST>
    class TCatmulRomDistSpline: public TSpline<VALUE>
    {
    public:
        TCatmulRomDistSpline(): m_length(0) {}

        VALUE interpolate(float position) const
        {
            //вернем что-нить
            if (m_values.size()<=0)
                return VALUE();
            if (m_values.size()==1)
                return m_values.front();

            if (position<=0)        return m_values.front();
            if (position>=length()) return m_values.back();

            float t = m_position2parameter(position);
            return m_spline.interpolate(t);
        }

        float length() const
        {
            return m_length;
        }

        void apply()
        {
            m_spline.m_values.swap(TCatmulRomSpline<VALUE>::List(m_values));
            m_position2parameter.m_values.swap(FloatLinearInterpolatorf::Map());
            m_length = 0;

            if (m_spline.m_values.size() < 1)
                return;

            TCatmulRomSpline<VALUE>::const_iterator i    =   m_spline.m_values.begin();
            TCatmulRomSpline<VALUE>::const_iterator last = --m_spline.m_values.end();
            const float step = 1.f/100.f;
            float t = 0;

            m_position2parameter.addValue(0,0);
            while (i != last)
            {
                for (float k=0; k<=1; k+=step)
                {
                    VALUE v1 = m_spline(t);
                    t+=step;
                    VALUE v2 = m_spline(t);

                    DIST Dist;
                    m_length += Dist(v1,v2);
                    m_position2parameter.addValue(m_length,t);
                }
                ++i;
            }
        }

        float getParameter (float position)
        {
            return m_position2parameter(position);
        }

    protected:
        void to_stream(io::write_stream& ws) const
        {
            ws << uint(m_spline.m_values.size());

            for(TCatmulRomSpline<VALUE>::const_iterator it = m_spline.m_values.begin(); it != m_spline.m_values.end(); ++it)
                ws << (*it);
        }

        void from_stream(io::read_stream& rs)
        {
            m_values.swap(List());

            uint num;
            rs >> num;
            for(uint i = 0; i < num; ++i)
            {
                VALUE value;
                rs >> value;
                m_values.push_back(value);
            }

            apply();
        }

    private:
        float                    m_length;             //длинна сплайна
        TCatmulRomSpline<VALUE>  m_spline;             //сплайн интерполируемых величин
        FloatLinearInterpolatorf m_position2parameter; //переход от параметризации длинной к параметризации по умолчанию
    };

} //namespace math