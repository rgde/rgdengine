//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 23.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Generator.h"
#include "Technique.h"
#include "StringConstants.h"
#include <boost/filesystem/operations.hpp>

CGenerator::CGenerator()
	: CGeneratorInput()
{
}

CGenerator::~CGenerator()
{
}

void CGenerator::generate()
{
	std::sort(m_pins.begin(), m_pins.end(), std::greater<CPin>());


	std::string outputDir = "Output";

	if(!getConstantValue("OutputDir", outputDir))
		base::lwrn<<"Constant \"OutputDir\" not found."<<
					" Using default output dir (\""<<outputDir<<
					"\").";

	bool placeIncludeAllInSubfolder = 
	base::Lexical_cast<bool, std::string>(
	getConstantValue("PlaceIncludeAllInSubfolder", "false")
										 );

	Combination combination;
	combination.resize(m_pins.size());

	makeAllCombinations(0, combination);

	std::string includeAllText;

	for(Files::const_iterator it = m_files.begin(); it != m_files.end(); it++)
	{
		std::string includeName;
		if(placeIncludeAllInSubfolder)
			includeName = it->first;
		else
			includeName = outputDir + "/" + it->first;
		includeAllText += makeIncludeText(includeName);

		saveFile(it->first, it->second, outputDir);
	}

	std::string includeAllOutDir;

	if(placeIncludeAllInSubfolder)
		includeAllOutDir = outputDir;

	saveFile(getConstantValue(IncludeAllConstantName, IncludeAllDefaultValue), includeAllText, includeAllOutDir, false);
}

void CGenerator::readInputFile(const std::string& fileName)
{
	CGeneratorInput::readInputFile(fileName);
}

bool CGenerator::getConstantValue(const std::string& name, std::string& value) const
{
	return m_constants.getConstantValue(name, value) ||
		   m_pinConstants.getConstantValue(name, value);
}

std::string CGenerator::getConstantValue(const std::string& name, const std::string& defaultValue) const
{
	std::string result = defaultValue;

	getConstantValue(name, result);

	return result;
}

void CGenerator::applyConstants(std::string& str) const
{
	m_constants.apply(str);
	m_pinConstants.apply(str);
}

std::string CGenerator::applyConstants(const std::string& str) const
{
	std::string result = str;
	applyConstants(result);
	return result;
}

const CGenerator::States& CGenerator::getStates() const
{
	return m_states;
}

void CGenerator::makeAllCombinations(uint n, Combination& combination)
{
	const CPin::Conditions& conditions = m_pins[n].getConditions();

	if(conditions.size() > 0)
		for(size_t i = 0; i < conditions.size(); i++)
		{
			combination[n] = conditions[i];
			if(n + 1 < m_pins.size())
				makeAllCombinations(n + 1, combination);
			else
				generateTechnique(combination);
		}
	else
		makeAllCombinations(n + 1, combination);
}

void CGenerator::generateTechnique(const Combination& combination)
{
	m_pinConstants.reset();

	for(size_t i = 0; i < combination.size(); i++)
		m_pinConstants.add(combination[i].getVariation().getConstants());

	CTechnique technique(combination);
	std::string strOutputName = technique.getName() + ".fx";

	std::string& file = m_files[strOutputName];

	if(file != "")
		if(file[file.size() - 1] == '\n')
			file += "\n";
		else
			file += "\n\n";

	file += applyConstants(technique.getText());
}

void CGenerator::saveFile(const std::string& fileName, const std::string& text, const std::string& dirName, bool addIncludes)
{
	std::string fullDirName = dirName;
	if(fullDirName != "")
	{
		fullDirName += "/";

		if(!boost::filesystem::exists(fullDirName))
			boost::filesystem::create_directory(fullDirName);
	}

	std::ofstream output;
	output.open((fullDirName + fileName).c_str(), std::ios_base::out | std::ios_base::trunc);
	output<<FileHeader;

	if(addIncludes)
		for(size_t i = 0; i < m_includes.size(); i++)
			output<<makeIncludeText(m_includes[i]);

	output<<"\n\n";

	output<<text;
}

std::string CGenerator::makeIncludeText(const std::string& fileName) const
{
	return "#include \"" + fileName + "\"\n";
}