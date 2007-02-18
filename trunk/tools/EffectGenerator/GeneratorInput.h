//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 05.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "XMLTypes.h"
#include "BaseStateValue.h"
#include "BaseState.h"
#include "Pin.h"
#include "Constants.h"

class CGeneratorInput
{
protected:

	CGeneratorInput();
	~CGeneratorInput();

	void readInputFile(const std::string& fileName);
	
public:

	typedef std::vector<PState> States;

protected:

	typedef std::vector<CPin> Pins;

	typedef std::string          Include;
	typedef std::vector<Include> Includes;

	typedef TAttributedParentElement<StateElement> GroupElement;

private:

	void readInputFile(const std::string& fileName, CPinPass::States& states);

	void readConstants(const TiXmlElement* elem);
	void readIncludes(const TiXmlElement* elem);
	void readStates(const TiXmlElement* elem, CPinPass::States& states);
	void readPins(const TiXmlElement* elem, CPinPass::States& states);
	void readInputs(const TiXmlElement* elem, CPinPass::States& states);

	void parseStatesGroup(const GroupElement& groupElem, CPinPass::States& states);

protected:

	CConstants m_constants;

	Includes   m_includes;

	States     m_states;
	Pins       m_pins;

};