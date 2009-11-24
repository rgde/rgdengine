//////////////////////////////////////////////////////////////////////////
//																		//
// example of use:														//
//		code:															//
//			LerpFloat lf;												//
//			lf.add_key(0, 0.0f);										//	
//			lf.add_key(1, 5.0f);										//
//			float result = lf.get_value(0.5f);							//
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
		struct key : public io::serialized_object
		{
		public:
			key(){}
			key(const T& v, float p) : value(v), position(p){}
			T value;
			float position;
			bool operator<(const key& k) {return position < k.position;}

		protected:
			void to_stream(io::write_stream& wf) const
			{
				wf	<< value
					<< position;
			}

			void from_stream(io::read_stream& rf)
			{
				rf	>> value >> position;
			}
		};

		typedef std::vector<key> keys_vector;

		interpolator() : m_sorted(false) {}

		T operator()(float t)
		{
			return get_value(t);
		}

		T get_value(float t)
		{
			if (!m_sorted)
				sort_keys();

			if (t > 1) t = 1.0f;
			else if (t < 0) t = 0;

			if (0 == m_keys.size())
			{
				return T();
			}

			key* less_eq	= getLessOrEqualKey(t);
			key* gr_eq		= getGreaterOrEqualKey(t);

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
			m_sorted = false;
			m_keys.push_back(key(value, pos));
		}

		keys_vector& get_keys()	{ return m_keys; }
		const keys_vector& get_keys() const	{ return m_keys; }

		void sort_keys(bool forceSort = false)
		{
			if (m_sorted && !forceSort) return;
			std::sort(m_keys.begin(), m_keys.end());
			m_sorted = true;
		}

		/// get less or equal key
		key* getLessOrEqualKey(float t) 
		{
			key* pk = 0;
			int size = (int)m_keys.size();
			for (int i = 0; i < size; ++i)
			{
				key& k = m_keys[i];
				if (k.position > t) return pk;
				pk = &(m_keys[i]);
			}
			return pk;
		}

		/// get greater or equal key
		key* getGreaterOrEqualKey(float t) 
		{
			key* pk = 0;
			int size = (int)m_keys.size();

			for (int i = size - 1; i >= 0 ; --i)
			{
				key& k = m_keys[i];
				if (k.position < t) return pk;
				pk = &(m_keys[i]);
			}
			return pk;
		}

	protected:
		virtual void to_stream(io::write_stream& wf) const
		{
			wf << (unsigned)m_keys.size();

			for( keys_vector::const_iterator it = m_keys.begin(); it != m_keys.end(); it++ )
				wf << (*it);
		}

		virtual void from_stream(io::read_stream& rf)
		{
			unsigned nKeys;
			rf >> nKeys;
			for( unsigned i = 0; i < nKeys; i++ )
			{
				key k;
				rf >> k;
				m_keys.push_back( k );
			}

			m_sorted = false;

			sort_keys(); // to be sure :)
		}

	private:
		bool m_sorted;
		mutable keys_vector m_keys;
	};

	typedef interpolator<int>		interpolatori;
	typedef interpolator<float>		interpolatorf;
	typedef interpolator<double>	interpolatord;


	template<class T = float, int Size = 3>
	class interpolator_vec : public io::serialized_object
	{
	public:
		typedef math::Vec<T, Size> Vec;

		interpolator_vec() : m_components(Size){}
		
		Vec operator()(float t)
		{
			return get_value(t);
		}

		Vec get_value(float t)
		{
			Vec out;

			for (int i = 0; i < Size; ++i)
				out[i] =  m_components[i](t);

			return out;
		}

		void add_key(float pos, const Vec& v)
		{
			for (int i = 0; i < Size; ++i)
				m_components[i].add_key(pos, v[i]);
		}

		interpolator<T>&  get_component(int component)
		{
            return m_components[component];
		}

		const interpolator<T>&  get_component(int component) const
		{
			return m_components[component];
		}

	protected:
		virtual void to_stream(io::write_stream& wf) const
		{
			wf << Size;

			for (int i = 0; i < Size; ++i)
				wf << m_components[i];
		}

		virtual void from_stream(io::read_stream& rf)
		{
			int size;
			rf >> size;
			if (size != Size)
			{
				throw std::bad_exception("Incorrect data.");
			}

			for (int i = 0; i < Size; ++i)
				rf >> m_components[i];
		}

	private:
		std::vector<interpolator<T> > m_components;
	};

	typedef interpolator_vec<float, 3>	interpolator_v3f;
	typedef interpolator_vec<float, 4>	interpolator_v4f;


	//--------------------------------------------------------------------------------------
	/// color interpolator
	class interpolator_col : public io::serialized_object
	{
	public:
		Color operator()(float t)
		{
			return get_value(t);
		}

		Color get_value(float t)
		{
			vec3f vColor = m_color(t);
			Color out( static_cast<unsigned char>(vColor[0]), static_cast<unsigned char>(vColor[1]),
				static_cast<unsigned char>(vColor[2]), static_cast<unsigned char>(m_alpha(t)) );
			return out;
		}

		void add_key(float pos, const Color& v)
		{
			m_color.add_key (pos, vec3f(v.r, v.g, v.b));
			m_alpha.add_key (pos, v.a);
		}

		interpolator_v3f&  get_color() { return m_color; }
		interpolatorf& get_alpha() { return m_alpha; }

		const interpolator_v3f&  get_color() const { return m_color; }
		const interpolatorf& get_alpha() const { return m_alpha; }

	protected:
		virtual void to_stream(io::write_stream& wf) const
		{
			wf << m_color << m_alpha;
		}

		virtual void from_stream(io::read_stream& rf)
		{
			rf >> m_color >> m_alpha;
		}

	private:
		interpolator_v3f m_color;
		interpolatorf m_alpha;
	};

	//template<class T>
	inline std::ostream& operator<<(std::ostream& out, const interpolatorf& interp)
	{
		const interpolatorf::keys_vector& keys = interp.get_keys();
		
		typedef interpolatorf::keys_vector::const_iterator const_iterator;

		for(const_iterator iter = keys.begin(); iter != keys.end(); ++iter)
		{
			out << base::lexical_cast<std::string, float>((*iter).position)	<< ","
				<< base::lexical_cast<std::string, float>((*iter).value)	<< ";";
		}
		return out;
	}
	
	inline std::ostream& operator<<(std::ostream& out, const interpolator_v3f& interp)
	{
		out << interp.get_component(0) << ";"
			<< interp.get_component(1) << ";"
			<< interp.get_component(2) << ";";
		return out;
	}

	typedef std::list<std::basic_string<char> > CharTokenList;

	inline std::istream& operator>>(std::istream& in, interpolatorf& interp)
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

	inline std::istream& operator>>(std::istream& in, interpolator_v3f& interp)
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
				stream >> interp.get_component(component++);
			else
				break;
		}

		return in;
	}

	inline std::ostream& operator<<(std::ostream& out, const interpolator_col& interp)
	{
		out << interp.get_color() << ";"
			<< interp.get_alpha() << ";";
		return out;
	}

	inline std::istream& operator>>(std::istream& in, interpolator_col& interp)
	{
		std::string data;
		in >> data;
		CharTokenList tokens = base::tokenize<char>(";;;", data);
		CharTokenList::iterator iter = tokens.begin();
		
		std::stringstream stream;
		stream << *iter << *(++iter);
		stream >> interp.get_color() >> interp.get_alpha();

		return in;
	}
}