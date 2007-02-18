//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 05.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GeneratorInput.h"
#include "VariableState.h"
#include "ArrayState.h"
#include "TurnState.h"
#include "VariableStateValue.h"
#include "ArrayStateValue.h"
#include "Generator.h"

CGeneratorInput::CGeneratorInput()
{
}

CGeneratorInput::~CGeneratorInput()
{
}

void CGeneratorInput::readInputFile(const std::string& fileName)
{
	CPinPass::States states;

	io::TheFileSystem::Get().setRootDir("");

	readInputFile(fileName, states);
}

void CGeneratorInput::readInputFile(const std::string& fileName, CPinPass::States& states)
{
	TiXmlDocument document;

	if(!base::loadXml(fileName, document))
	{
		base::lerr<<"Can't open input XML file \""<<fileName<<"\".";
		return;
	}

	TiXmlElement* root = document.FirstChildElement("EffectGeneratorParameters");

	if(!root)
	{
		base::lerr<<"Can't find root element in input XML file \""<<fileName<<"\".";
		return;
	}

	TiXmlElement* elem = root->FirstChildElement();

	while(elem != NULL)
	{
		std::string name = elem->Value();

		if(name == "constants")
			readConstants(elem);
		else
		if(name == "includes")
			readIncludes(elem);
		else
		if(name == "states")
			readStates(elem, states);
		else
		if(name == "pins")
			readPins(elem, states);
		else
		if(name == "inputs")
			readInputs(elem, states);
		else
			base::lwrn<<"Found element \""<<name<<"\" while parsing input file \""<<
						fileName<<"\". Nothing know about this type. Skipping...";

		elem = elem->NextSiblingElement();
	}
}

void CGeneratorInput::readConstants(const TiXmlElement* elem)
{
	m_constants.add(CConstants(ConstantsElement(elem)));
}

void CGeneratorInput::readIncludes(const TiXmlElement* elem)
{
	const TiXmlElement* first   = elem->FirstChildElement();
	const TiXmlElement* current = first;

	while(current != NULL)
	{
		CLeafElement leaf(current);

		std::string name = leaf.getName();

		if(name == "include")
			m_includes.push_back(TheGenerator::Get().applyConstants(leaf.getValue()));
		else
			base::lwrn<<"Unknown element \""<<name<<
						"\" in includes block (element \""<<
						elem->Value()<<"\").";

		current = current->NextSiblingElement();
	}
}

void CGeneratorInput::readStates(const TiXmlElement* elem, CPinPass::States& states)
{
	const TiXmlElement* first   = elem->FirstChildElement();
	const TiXmlElement* current = first;

	while(current != NULL)
	{
		GroupElement groupElem(current);

		parseStatesGroup(groupElem, states);

		current = current->NextSiblingElement();
	}
}

void CGeneratorInput::readPins(const TiXmlElement* elem, CPinPass::States& states)
{
	const TiXmlElement* first   = elem->FirstChildElement();
	const TiXmlElement* current = first;

	while(current != NULL)
	{
		std::string name = current->Value();

		if(name == "pin")
			m_pins.push_back(CPin(current, states));
		else
			base::lwrn<<"Unknown element \""<<name<<
						"\" in pins block (element \""<<
						name<<"\").";

		current = current->NextSiblingElement();
	}
}

void CGeneratorInput::readInputs(const TiXmlElement* elem, CPinPass::States& states)
{
	const TiXmlElement* first   = elem->FirstChildElement();
	const TiXmlElement* current = first;

	while(current != NULL)
	{
		CLeafElement leaf(current);

		std::string name = leaf.getName();

		if(name == "file")
			readInputFile(TheGenerator::Get().applyConstants(leaf.getValue()), states);
		else
			base::lwrn<<"Unknown element \""<<name<<
						"\" in inputs block (element \""<<
						elem->Value()<<"\").";

		current = current->NextSiblingElement();
	}
}

void CGeneratorInput::parseStatesGroup(const GroupElement& groupElem, CPinPass::States& states)
{
	const GroupElement::LeafElements& elems = groupElem.getChildren();

	std::string typeAttribute = "variable";
	if(!groupElem.findAttribute("type", typeAttribute))
		base::lwrn<<"GroupElement \""<<groupElem.getName()<<
					"\" has no information about it's type. "
					"Using default type \""<<typeAttribute<<
					"\".";
	else
		TheGenerator::Get().applyConstants(typeAttribute);

	uint max = 0;

	if(typeAttribute == "array" || typeAttribute == "turn")
	{
		std::string strMax;

		if(groupElem.findAttribute("max", strMax))
		{
			TheGenerator::Get().applyConstants(strMax);
			max = base::Lexical_cast<int, std::string>(strMax);
		}
		else
			base::lwrn<<"GroupElement \""<<groupElem.getName()<<
						"\" has no information about max size. Using "<<max<<"...";
	}


	for(size_t i = 0; i < elems.size(); i++)
	{
		const StateElement& stateElem = elems[i];

		IBaseState* pState = NULL;

		if((typeAttribute == "variable"))
		{
			IVariableState* pVariableState = new CVariableState(stateElem);
			pState = pVariableState;
			states.variableStates.insert(CPinPass::States::VariableStates::value_type(pVariableState->getName(), pVariableState));
		}
		else
		if(typeAttribute == "array")
		{
			IArrayState* pArrayState = new CArrayState(stateElem, max);
			pState = pArrayState;
			states.arrayStates.insert(CPinPass::States::ArrayStates::value_type(pArrayState->getName(), pArrayState));
		}
		else
		if(typeAttribute == "turn")
		{
			IVariableState* pVariableState = new CTurnState(stateElem, max);
			pState = pVariableState;
			states.variableStates.insert(CPinPass::States::VariableStates::value_type(pVariableState->getName(), pVariableState));
		}
		else
			base::lwrn<<"Group \""<<groupElem.getName()<<
						"\" has unknown type \""<<typeAttribute<<
						"\". Skipping...";

		if(NULL != pState)
			m_states.push_back(PState(pState));
	}
}