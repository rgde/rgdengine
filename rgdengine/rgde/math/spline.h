#pragma once

#include <rgde/io/io.h>

namespace math
{

    template <typename VALUE>
    class TSpline : public io::serialized_object
    {
    public:
        typedef std::list<VALUE> List;
        typedef typename List::iterator iterator;
        typedef typename List::const_iterator const_iterator;

        TSpline() {}
        virtual ~TSpline() {}

        List m_values;

        VALUE operator()(float position) const {return interpolate(position);}
        virtual VALUE interpolate(float position) const = 0;
        virtual float length() const = 0;

        //эти методы могут потребоваться в будущем [Dub]
        virtual void apply() {} //уже понадобился [Dub]
        //virtual void undo() {}

    protected:
        void toStream(io::IWriteStream& ws) const;
        void fromStream(io::IReadStream& rs);

    private:
        TSpline(const TSpline&);
        TSpline& operator=(const TSpline&);
    };

    template <typename VALUE>
    void TSpline<VALUE>::toStream(io::IWriteStream& ws) const
    {
        ws << uint(m_values.size());

        for(const_iterator it = m_values.begin(); it != m_values.end(); ++it)
            ws << (*it);
    }

    template <typename VALUE>
    void TSpline<VALUE>::fromStream(io::IReadStream& rs)
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