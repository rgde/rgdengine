//interpolator.h
#pragma once

namespace math
{
    //добавляя значение (value_t), сразу указываем ее положение (param_t) относительно других значений
    template <typename value_t, typename param_t>
    class base_interpolator : public io::serialized_object
    {
    public:
        typedef std::map<param_t, value_t> keys;
        typedef typename keys::value_type value_type;
        typedef typename keys::iterator iterator;
        typedef typename keys::const_iterator const_iterator;

        base_interpolator() {}
        virtual ~base_interpolator() {}

        keys m_keys;

        void add(param_t position, value_t value);
        void scale(param_t begin, param_t end);
        const_iterator getLessOrEqualIterator(param_t position) const;
        const_iterator getGreaterOrEqualIterator(param_t position) const;

        value_t operator()(param_t position) const {return interpolate(position);}
        virtual value_t interpolate(param_t position) const = 0;

    protected:
        void to_stream(io::write_stream& ws) const;
        void from_stream(io::read_stream& rs);

    private:
        base_interpolator(const base_interpolator&);
        base_interpolator& operator=(const base_interpolator&);
    };

    template <typename value_t, typename param_t>
    void base_interpolator<value_t, param_t>::add(param_t position, value_t value)
    {
        m_keys.insert(value_type(position,value));
    }

    template <typename value_t, typename param_t>
    void base_interpolator<value_t, param_t>::scale(param_t begin, param_t end)
    {
        if (m_keys.size() < 2)
            return;

        param_t min = m_keys.begin()->first;
        param_t max = m_keys.rbegin()->first;

        keys tmp;
        for (iterator it = m_keys.begin(); it != m_keys.end(); ++it)
		{
            tmp.insert(value_type((it->first-min)/(max-min) * (end-begin) + begin, it->second))
		}

        m_keys.swap(tmp);
    }

    template <typename value_t, typename param_t>
    typename base_interpolator<value_t, param_t>::const_iterator base_interpolator<value_t, param_t>::getLessOrEqualIterator(param_t position) const
    {
        const_iterator result = m_keys.end();

        for(const_iterator it = m_keys.begin(); it != m_keys.end(); ++it)
        {
            if(it->first > position)
                break;
            else
                result = it;
        }
        return result;
    }

    template <typename value_t, typename param_t>
    typename base_interpolator<value_t, param_t>::const_iterator base_interpolator<value_t, param_t>::getGreaterOrEqualIterator(param_t position) const
    {
        for(const_iterator it = m_keys.begin(); it != m_keys.end(); ++it)
            if(it->first >= position)
                return it;
        return m_keys.end();
    }

    template <typename value_t, typename param_t>
    void base_interpolator<value_t, param_t>::to_stream(io::write_stream& ws) const
    {
        ws << uint(m_keys.size());

        for(const_iterator it = m_keys.begin(); it != m_keys.end(); ++it)
        {		
            ws << it->first;
            ws << it->second;
        }
    }

    template <typename value_t, typename param_t>
    void base_interpolator<value_t, param_t>::from_stream(io::read_stream& rs)
    {
        m_keys.clear();

        uint num;
        rs >> num;
        for(uint i = 0; i < num; ++i)
        {
            param_t position;
            value_t value;
            rs >> position;
            rs >> value;
            add(position, value);
        }
    }
} //namespace math