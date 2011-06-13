#pragma once

#include <rgde/io/io.h>

namespace math
{
    template <typename value_t>
    class spline : public io::serialized_object
    {
    public:
        typedef std::list<value_t> keys;
        typedef typename keys::iterator iterator;
        typedef typename keys::const_iterator const_iterator;

        spline() {}
        virtual ~spline() {}

        keys m_keys;

        value_t operator()(float position) const {return interpolate(position);}

        virtual value_t interpolate(float position) const = 0;
        virtual float length() const = 0;

        virtual void apply() {}

    protected:
        void to_stream(io::write_stream& ws) const;
        void from_stream(io::read_stream& rs);

    private:
        spline(const spline&);
        spline& operator=(const spline&);
    };

    template <typename value_t>
    void spline<value_t>::to_stream(io::write_stream& ws) const
    {
        ws << uint(m_keys.size());

        for(const_iterator it = m_keys.begin(); it != m_keys.end(); ++it)
            ws << (*it);
    }

    template <typename value_t>
    void spline<value_t>::from_stream(io::read_stream& rs)
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
    }

} //namespace math