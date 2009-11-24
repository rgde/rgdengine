#pragma once

#include <rgde/io/io.h>

namespace math
{
    template <typename VALUE>
    class spline : public io::serialized_object
    {
    public:
        typedef std::list<VALUE> List;
        typedef typename List::iterator iterator;
        typedef typename List::const_iterator const_iterator;

        spline() {}
        virtual ~spline() {}

        List m_values;

        VALUE operator()(float position) const {return interpolate(position);}

        virtual VALUE interpolate(float position) const = 0;
        virtual float length() const = 0;

        virtual void apply() {}

    protected:
        void to_stream(io::write_stream& ws) const;
        void from_stream(io::read_stream& rs);

    private:
        spline(const spline&);
        spline& operator=(const spline&);
    };

    template <typename VALUE>
    void spline<VALUE>::to_stream(io::write_stream& ws) const
    {
        ws << uint(m_values.size());

        for(const_iterator it = m_values.begin(); it != m_values.end(); ++it)
            ws << (*it);
    }

    template <typename VALUE>
    void spline<VALUE>::from_stream(io::read_stream& rs)
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
    }

} //namespace math