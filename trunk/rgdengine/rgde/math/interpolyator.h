//////////////////////////////////////////////////////////////////////////
//																		//
// example of use:														//
//		code:															//
//			LerpFloat lf;												//
//			lf.add_key(0, 0.0f);											//	
//			lf.add_key(1, 5.0f);											//
//			float result = lf.getValue(0.5f);							//
//																		//
//		result:															//
//			2.5															//
//																		//
//////////////////////////////////////////////////////////////////////////
#pragma once


#include <rgde/io/io.h>
#include <rgde/base/lexical_cast.h>

/**
Namespace for different math stuff
*/
namespace math
{

	/**
	class interpolator
	used to give smooth linear interpolation different Keys
	at given parameter (time) t
	*/
	template<class T = float>
	class interpolator : public io::serialized_object
	{
	public:
		struct Key : public io::serialized_object
		{
		public:
			Key(){}
			Key(const T& v, float p) : value(v), position(p){}
			T value;
			float position;
			bool operator<(const Key& k) {return position < k.position;}

		protected:
			void toStream(io::write_stream& wf) const
			{
				wf	<< value
					<< position;
			}

			void fromStream(io::read_stream& rf)
			{
				rf	>> value >> position;
			}
		};

		typedef std::vector<Key> Keys;

	public:
		interpolator():m_bSorted(false) {}

		T operator()(float t)
		{
			return getValue(t);
		}

		T getValue(float t)
		{
			if (!m_bSorted)
				sortVector();

			//if (t > 1 || t < 0)
			//{
			//	throw(std::exception("effect_param_impl out of range !"));
			//}
			if (t > 1) t = 1.0f;
			else if (t < 0) t = 0;

			if (0 == m_vKeys.size())
			{
				return T();
			}

			Key* less_eq	= getLessOrEqualKey(t);
			Key* gr_eq		= getGreaterOrEqualKey(t);

			if (less_eq == gr_eq)
			{
				return less_eq->value;
			}
			else if (0 == less_eq)
			{
				return gr_eq->value;
			}
			else if(0 == gr_eq)
			{
				return less_eq->value;
			}
			else
			{
				float dt = gr_eq->position - less_eq->position;
				float rt = t - less_eq->position;
				T dvalue = gr_eq->value - less_eq->value;
				return dvalue * (rt / dt) + less_eq->value;
			}
		}

		void add_key(float pos, const T& value)
		{
			m_bSorted = false;
			m_vKeys.push_back(Key(value, pos));
		}

		Keys& getKeys()
		{
            return m_vKeys;
		}

		const Keys& getKeys() const
		{
			return m_vKeys;
		}

		void sortVector(bool forceSort = false)
		{
			if (m_bSorted && !forceSort) return;
			std::sort(m_vKeys.begin(), m_vKeys.end());
			m_bSorted = true;
		}

		Key* getLessOrEqualKey(float t) 
		{
			Key* pk = 0;
			int size = (int)m_vKeys.size();
			for (int i = 0; i < size; ++i)
			{
				Key& k = m_vKeys[i];
				if (k.position > t) return pk;
				pk = &(m_vKeys[i]);
			}
			return pk;
		}

		Key* getGreaterOrEqualKey(float t) 
		{
			Key* pk = 0;
			int size = (int)m_vKeys.size();

			for (int i = size - 1; i >= 0 ; --i)
			{
				Key& k = m_vKeys[i];
				if (k.position < t) return pk;
				pk = &(m_vKeys[i]);
			}
			return pk;
		}

	protected:
		virtual void toStream(io::write_stream& wf) const
		{
			//sortVector();		//save only sorted data
			// ^^^ - now we perform sorting on load

			wf << (unsigned)m_vKeys.size();

			for( Keys::const_iterator it = m_vKeys.begin(); it != m_vKeys.end(); it++ )
				wf << (*it);
		}

		virtual void fromStream(io::read_stream& rf)
		{
			unsigned nKeys;
			rf >> nKeys;
			for( unsigned i = 0; i < nKeys; i++ )
			{
				Key key;
				rf >> key;
				m_vKeys.push_back( key );
			}

			m_bSorted = false;

			sortVector(); // to be sure
		}

	private:
		bool m_bSorted;
		mutable Keys m_vKeys;
	};

	typedef interpolator<int>		IntInterp;
	typedef interpolator<float>	FloatInterp;
	typedef interpolator<double>	DoubleInterp;


	template<class T = float, int Size = 3>
	class TVectorInterpolator : public io::serialized_object
	{
	public:
		typedef math::Vec<T, Size> Vec;

		TVectorInterpolator() : m_vComponents(Size){}
		
		Vec operator()(float t)
		{
			return getValue(t);
		}

		Vec getValue(float t)
		{
			Vec out;

			for (int i = 0; i < Size; ++i)
				out[i] =  m_vComponents[i](t);

			return out;
		}

		void add_key(float pos, const Vec& v)
		{
			for (int i = 0; i < Size; ++i)
				m_vComponents[i].add_key(pos, v[i]);
		}

		interpolator<T>&  getComponent(int component)
		{
            return m_vComponents[component];
		}

		const interpolator<T>&  getComponent(int component) const
		{
			return m_vComponents[component];
		}

	protected:
		virtual void toStream(io::write_stream& wf) const
		{
			wf << Size;

			for (int i = 0; i < Size; ++i)
				wf << m_vComponents[i];
		}

		virtual void fromStream(io::read_stream& rf)
		{
			int size;
			rf >> size;
			if (size != Size)
			{
				throw std::bad_exception("Incorrect data.");
			}

			for (int i = 0; i < Size; ++i)
				rf >> m_vComponents[i];
		}

	private:
		std::vector<interpolator<T> > m_vComponents;
	};

	typedef TVectorInterpolator<float, 3>	Vec3Interp;
	typedef TVectorInterpolator<float, 4>	Vec4Interp;


	//--------------------------------------------------------------------------------------
	// Интерполятор цвета
	class ColorInterp : public io::serialized_object
	{
	public:
		Color operator()(float t)
		{
			return getValue(t);
		}

		Color getValue(float t)
		{
			Vec3f vColor = m_vColor(t);
			Color out( static_cast<unsigned char>(vColor[0]), static_cast<unsigned char>(vColor[1]),
				static_cast<unsigned char>(vColor[2]), static_cast<unsigned char>(m_Alpha(t)) );
			return out;
		}

		void add_key(float pos, const Color& v)
		{
			m_vColor.add_key (pos, Vec3f(v.r, v.g, v.b));
			m_Alpha.add_key (pos, v.a);
		}

		Vec3Interp&  getColor() { return m_vColor; }
		FloatInterp& getAlpha() { return m_Alpha; }

		const Vec3Interp&  getColor() const { return m_vColor; }
		const FloatInterp& getAlpha() const { return m_Alpha; }

	protected:
		virtual void toStream(io::write_stream& wf) const
		{
			wf << m_vColor << m_Alpha;
		}

		virtual void fromStream(io::read_stream& rf)
		{
			rf >> m_vColor >> m_Alpha;
		}

	private:
		Vec3Interp m_vColor;
		FloatInterp m_Alpha;
	};

	//template<class T>
	inline std::ostream& operator<<(std::ostream& out, const FloatInterp& interp)
	{
		const FloatInterp::Keys& keys = interp.getKeys();
		for(FloatInterp::Keys::const_iterator iter = keys.begin(); iter != keys.end(); ++iter)
		{
			out << base::lexical_cast<std::string, float>((*iter).position)	<< ","
				<< base::lexical_cast<std::string, float>((*iter).value)	<< ";";
		}
		return out;
	}
	
	inline std::ostream& operator<<(std::ostream& out, const Vec3Interp& interp)
	{
		out << interp.getComponent(0) << ";"
			<< interp.getComponent(1) << ";"
			<< interp.getComponent(2) << ";";
		return out;
	}

	typedef std::list<std::basic_string<char> > CharTokenList;

	inline std::istream& operator>>(std::istream& in, FloatInterp& interp)
	{
		std::string data;
		in >> data;
		CharTokenList tokens = base::tokenize<char>(";", data);
		for (CharTokenList::iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
		{
			CharTokenList elem = base::tokenize<char>(",", *iter);
			
			CharTokenList::iterator it = elem.begin(); // assume that stored position and value only
			float position = base::lexical_cast<float, std::string>(*it);
			float value = base::lexical_cast<float, std::string>(*(++it));
			interp.add_key(position, value);
		}

		return in;
	}

	inline std::istream& operator>>(std::istream& in, Vec3Interp& interp)
	{
		int component = 0;
		std::string data;
		in >> data;
		CharTokenList tokens = base::tokenize<char>(";;", data);
		for (CharTokenList::iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
		{
			std::stringstream stream;
			stream << *iter;
			if(component < 3)
				stream >> interp.getComponent(component++);
			else
				break;
		}

		return in;
	}

	inline std::ostream& operator<<(std::ostream& out, const ColorInterp& interp)
	{
		out << interp.getColor() << ";"
			<< interp.getAlpha() << ";";
		return out;
	}

	inline std::istream& operator>>(std::istream& in, ColorInterp& interp)
	{
		std::string data;
		in >> data;
		CharTokenList tokens = base::tokenize<char>(";;;", data);
		CharTokenList::iterator iter = tokens.begin();
		
		std::stringstream stream;
		stream << *iter << *(++iter);
		stream >> interp.getColor() >> interp.getAlpha();

		return in;
	}
}