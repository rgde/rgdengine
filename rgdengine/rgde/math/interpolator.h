//interpolator.h
#pragma once

namespace math
{

    //добавляя значение (VALUE), сразу указываем ее положение (POSITION) относительно других значений
    template <typename VALUE, typename POSITION>
    class TInterpolatorEx: public io::serialized_object
    {
    public:
        typedef std::map<POSITION, VALUE> Map;
        typedef typename Map::value_type value_type;
        typedef typename Map::iterator iterator;
        typedef typename Map::const_iterator const_iterator;

        TInterpolatorEx() {}
        virtual ~TInterpolatorEx() {}

        Map m_values;

        void addValue(POSITION position, VALUE value);
        void scale(POSITION begin, POSITION end);
        const_iterator getLessOrEqualIterator(POSITION position) const;
        const_iterator getGreaterOrEqualIterator(POSITION position) const;

        VALUE operator()(POSITION position) const {return interpolate(position);}
        virtual VALUE interpolate(POSITION position) const = 0;

    protected:
        void toStream(io::IWriteStream& ws) const;
        void fromStream(io::IReadStream& rs);

    private:
        TInterpolatorEx(const TInterpolatorEx&);
        TInterpolatorEx& operator=(const TInterpolatorEx&);
    };

    template <typename VALUE, typename POSITION>
    void TInterpolatorEx<VALUE, POSITION>::addValue(POSITION position, VALUE value)
    {
        m_values.insert(value_type(position,value));
    }

    template <typename VALUE, typename POSITION>
    void TInterpolatorEx<VALUE, POSITION>::scale(POSITION begin, POSITION end)
    {
        if (m_values.size() < 2)
            return;

        POSITION min = m_values.begin()->first;
        POSITION max = m_values.rbegin()->first;

        Map tmp;
        for (iterator it = m_values.begin(); it != m_values.end(); ++it)
            tmp.insert(value_type((it->first-min)/(max-min) * (end-begib) + begin, it->second))
        m_values.swap(tmp);
    }

    template <typename VALUE, typename POSITION>
    typename TInterpolatorEx<VALUE, POSITION>::const_iterator TInterpolatorEx<VALUE, POSITION>::getLessOrEqualIterator(POSITION position) const
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
    typename TInterpolatorEx<VALUE, POSITION>::const_iterator TInterpolatorEx<VALUE, POSITION>::getGreaterOrEqualIterator(POSITION position) const
    {
        for(const_iterator it = m_values.begin(); it != m_values.end(); ++it)
            if(it->first >= position)
                return it;
        return m_values.end();
    }

    template <typename VALUE, typename POSITION>
    void TInterpolatorEx<VALUE, POSITION>::toStream(io::IWriteStream& ws) const
    {
        ws << uint(m_values.size());

        for(const_iterator it = m_values.begin(); it != m_values.end(); ++it)
        {		
            ws << it->first;
            ws << it->second;
        }
    }

    template <typename VALUE, typename POSITION>
    void TInterpolatorEx<VALUE, POSITION>::fromStream(io::IReadStream& rs)
    {
        m_values.swap(Map());

        uint num;
        rs >> num;
        for(uint i = 0; i < num; ++i)
        {
            POSITION position;
            VALUE value;
            rs >> position;
            rs >> value;
            addValue(position, value);
        }
    }

} //namespace math