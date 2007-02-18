//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 23.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "GeneratorInput.h"

class CGenerator : public CGeneratorInput
{
public:

	void readInputFile(const std::string& fileName);

	void generate();

	bool getConstantValue(const std::string& name, std::string& value) const;
	std::string getConstantValue(const std::string& name, const std::string& defaultValue) const;

	std::string applyConstants(const std::string& str) const;
	void applyConstants(std::string& str) const;

	const States& getStates() const;

	std::string makeIncludeText(const std::string& fileName) const;

protected:

	CGenerator();
	~CGenerator();

private:

	typedef std::map<std::string, std::string> Files;

private:

	void makeAllCombinations(uint n, Combination& combination);
	void generateTechnique(const Combination& combination);

	void saveFile(const std::string& fileName, const std::string& text,
				  const std::string& dirName, bool addIncludes = true);

private:

	Files      m_files;
	CConstants m_pinConstants;

};

typedef base::TSingelton<CGenerator> TheGenerator;