//======== Copyright © 2004 - 2005, viv software, All rights reserved =========
//
// File: dator.h
// Author: Vashchaev 'viv' Ivan
// Purpose: $$$
//
//=============================================================================

#ifndef BASE_DATOR_H
#define BASE_DATOR_H

#include "lexical_cast.h"

namespace base
{
	class Dator
	{
	protected:
		struct Holder
		{
			virtual void operator=(const std::string &s) = 0;
			virtual std::string ToString() const = 0;
			virtual Holder *Clone() const = 0;
		};

		template<typename T> 
		struct HolderT : Holder
		{
			T *Ptr;

			HolderT(T *p): Ptr(p)
			{
			}

			void operator=(const std::string &s)
			{
				*Ptr = Lexical_cast<T>(s);
			}

			std::string ToString() const
			{
				return Lexical_cast<std::string>(*Ptr);
			}

			Holder *Clone() const
			{
				return new HolderT(Ptr);
			};
		};

		Holder *m_Holder;

	public:
		Dator();

		Dator(const Dator &v);

		template<typename T> explicit Dator(T *p): m_Holder(new HolderT<T>(p))
		{
		}

		virtual ~Dator();

		Dator &operator=(const Dator &v);

		bool Empty() const;

		Dator &operator=(const std::string &s);

		std::string ToString() const;
	};
}

#endif