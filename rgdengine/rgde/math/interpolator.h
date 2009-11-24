//interpolator.h
#pragma once

namespace math
{
    //добавляя значение (VALUE), сразу указываем ее положение (POSITION) относительно других значений
    template <typename VALUE, typename POSITION>
    class interpolator_ex : public io::serialized_object
    {
    public:
        typedef std::map<POSITION, VALUE> keys_map;
        typedef typename keys_map::value_type value_type;
        typedef typename keys_map::iterator iterator;
        typedef typename keys_map::const_iterator const_iterator;

        interpolator_ex() {}
        virtual ~interpolator_ex() {}

        keys_map m_values;

        void add_value(POSITION position, VALUE value);
        void scale(POSITION begin, POSITION end);
        const_iterator getLessOrEqualIterator(POSITION position) const;
        const_iterator getGreaterOrEqualIterator(POSITION position) const;

        VALUE operator()(POSITION position) const {return interpolate(position);}
        virtual VALUE interpolate(POSITION position) const = 0;

    protected:
        void to_stream(io::write_stream& ws) const;
        void from_stream(io::read_stream& rs);

    private:
        interpolator_ex(const interpolator_ex&);
        interpolator_ex& operator=(const interpolator_ex&);
    };

    template <typename VALUE, typename POSITION>
    void interpolator_ex<VALUE, POSITION>::add_value(POSITION position, VALUE value)
    {
        m_values.insert(value_type(position,value));
    }

    template <typename VALUE, typename POSITION>
    void interpolator_ex<VALUE, POSITION>::scale(POSITION begin, POSITION end)
    {
        if (m_values.size() < 2)
            return;

        POSITION min = m_values.begin()->first;
        POSITION max = m_values.rbegin()->first;

        keys_map tmp;
        for (iterator it = m_values.begin(); it != m_values.end(); ++it)
		{
            tmp.insert(value_type((it->first-min)/(max-min) * (end-begin) + begin, it->second))
		}

        m_values.swap(tmp);
    }

    template <typename VALUE, typename POSITION>
    typename interpolator_ex<VALUE, POSITION>::const_iterator interpolator_ex<VALUE, POSITION>::getLessOrEqualIterator(POSITION position) const
    {
        const_iterator result = m_values.end();

        for(const_iterator it = m_values.begin(); it != m_values.end(); ++it)
        {
            if(it->first > position)
                break;
            else
                result = it;
        }
        return result;
    }

    template <typename VALUE, typename POSITION>
    typename interpolator_ex<VALUE, POSITION>::const_iterator interpolator_ex<VALUE, POSITION>::getGreaterOrEqualIterator(POSITION position) const
    {
        for(const_iterator it = m_values.begin(); it != m_values.end(); ++it)
            if(it->first >= position)
                return it;
        return m_values.end();
    }

    template <typename VALUE, typename POSITION>
    void interpolator_ex<VALUE, POSITION>::to_stream(io::write_stream& ws) const
    {
        ws << uint(m_values.size());

        for(const_iterator it = m_values.begin(); it != m_values.end(); ++it)
        {		
            ws << it->first;
            ws << it->second;
        }
    }

    template <typename VALUE, typename POSITION>
    void interpolator_ex<VALUE, POSITION>::from_stream(io::read_stream& rs)
    {
        m_values.swap(keys_map());

        uint num;
        rs >> num;
        for(uint i = 0; i < num; ++i)
        {
            POSITION position;
            VALUE value;
            rs >> position;
            rs >> value;
            add_value(position, value);
        }
    }

} //namespace math