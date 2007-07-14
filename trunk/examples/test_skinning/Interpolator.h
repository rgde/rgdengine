/*

@author PC
*/
#pragma once

/* Key class. Here 'key' is synonym to 'value', i.e. key
   contains some value which is used for interpolating.
   !TValue is a type of 'value to interpolate'. */
template <class TValue>
class Key : public io::ISerializedObject
{
public:

	Key() : m_value()	{ }
	Key(const TValue& value) : m_value(value) { }

	const TValue& getValue() const { return m_value; }
	void          setValue(const TValue& value) { m_value = value; }

protected:

	void toStream(io::IWriteStream& ws) const
	{
		ws	<< m_value;
	}

	void fromStream(io::IReadStream& rs)
	{
		rs	>> m_value;
	}

private:

	TValue m_value;
};

/* Base class for interpolation methods. Subclasses of this
   class have to implement 'interpolate' method.
   !TValues is the same as in 'Key' class.
   !TPosition is type of 'keys values' i.e. values which
   are used to set 'value to interpolate' positions. Typically
   TPosition is 'float' but you can use another type
   ('double', for example). */
template <class TValue, class TPosition>
class TInterpolationMethod
{
protected:

	typedef Key<TValue> MyKey;
	typedef std::map<TPosition, MyKey> Keys;
	typedef typename Keys::iterator KeysIter;
	typedef typename Keys::const_iterator KeysConstIter;

protected:

	TInterpolationMethod(const Keys& keys) : m_keys(keys) {}

	virtual TValue interpolate(const TPosition& position) const = 0;

	virtual void keysUpdated()
	{
		;
	}

	KeysConstIter getLessOrEqualKey(const TPosition& position) const
	{
		KeysConstIter r = m_keys.end();

		for(KeysConstIter it = m_keys.begin(); it != m_keys.end(); it++)
		{
			if(it->first > position)
				break;
			else
				r = it;
		}
		return r;
	}

	KeysConstIter getGreaterOrEqualKey(const TPosition& position) const
	{
		for(KeysConstIter it = m_keys.begin(); it != m_keys.end(); it++)
			if(it->first >= position)
				return it;
		return m_keys.end();
	}

protected:

	const Keys& m_keys;

};

/* Interpolator class. This class is template so you can make
   all possible interpolators using different template parameters.
   !TValue and !TPosition are the same as in TInterpolationMethod
   class.
   !TMethod has to be subclass of TInterpolationMethod<TValue, TPosition>
   class. Certainly !TValue and !TPosition parameters have to be the same
   for TInterpolator and TInterpolationMethod classes.
*/
template <class TValue, class TPosition, class TMethod>
class TInterpolator : public io::ISerializedObject,
					  public TMethod
{
public:

	typedef Key<TValue> MyKey;

private:

	typedef std::map<TPosition, MyKey> Keys;

public:

	TInterpolator() : TMethod(m_keys) {}
	TInterpolator(const TInterpolator& inter) : m_keys(inter.m_keys), TMethod(m_keys) {}
	~TInterpolator() {}

	inline TInterpolator& operator =(const TInterpolator& inter)
	{
		*this = TInterpolator(inter);
		return *this;
	}

	TValue operator() (const TPosition& position) const { return getValue(position); }

	TValue getValue(const TPosition& position) const
	{
		if(0 == m_keys.size())
			return TValue();

		return TMethod::interpolate(position);
	}

	/* Normalizes keys' positions */
	void normalize()
	{
		TPosition max = m_keys.rend()->first,
			      imax = TPosition(1)/max;

		for(Keys::iterator it = m_keys.begin(); it != m_keys.end(); it++)
			it->first *= imax;
	}

	void addKey(const TPosition& position, const TValue& value) { addKey(position, MyKey(value)); }
	void addKey(const TPosition& position, const MyKey& key) { m_keys.insert(Keys::value_type(position, key)); keysUpdated(); }

	//TODO:Make methods to acess to keys...

private:

	void keysUpdated() { TMethod::keysUpdated(); }

	void toStream(io::IWriteStream& ws) const
	{
		ws << uint(m_keys.size());

		for(Keys::const_iterator it = m_keys.begin(); it != m_keys.end(); it++)
		{		
			ws << it->first;
			ws << it->second;
		}
	}

	void fromStream(io::IReadStream& rs)
	{
		uint num;
		rs >> num;
		for(uint i = 0; i < num; i++)
		{
			TPosition  position;
			MyKey key;
			rs >> position;
			rs >> key;
			addKey(position, key);
		}
	}

private:

	Keys m_keys;

};