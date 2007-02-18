//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 02.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseXMLTypes.h"

template <class TLeafType>
class TParentElement : public IBaseElement
{
public:

	typedef std::vector<TLeafType> LeafElements;

public:

	TParentElement(const TiXmlElement* elem)
		: IBaseElement(elem)
	{
		const TiXmlElement* first   = elem->FirstChildElement();
		const TiXmlElement* current = first;

		while(current != NULL)
		{
			m_children.push_back(TLeafType(current));
			current = current->NextSiblingElement();
		}
	}

	const LeafElements& getChildren() const
	{
		return m_children;
	}

private:

	LeafElements m_children;

};

template <class TLeafType>
class TAttributedParentElement : public TParentElement<TLeafType>, public IHasAttributes
{
public:

	TAttributedParentElement(const TiXmlElement* elem)
		: TParentElement(elem), IHasAttributes(elem)
	{
	}

};