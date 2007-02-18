//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 01.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Pass.h"
#include "Condition.h"

class CTechnique
{
public:

	CTechnique(const Combination& combination);
	~CTechnique();


	std::string getName() const;
	std::string getText() const;

private:

	void setCurrentPass(uint n);
	CPass& getCurrentPass();
	CPass& addNewPass(const CPass& pass);
	CPass& addNewPass();


	void addName(const std::string& nameAdd);

	void reset();

	void beginPass() const;
	void endPass(CPass& pass) const;

private:

	typedef std::vector<CPass> Passes;
	typedef std::vector<std::string> NameAdds;

private:

	Passes   m_passes;
	NameAdds m_nameAdds;
	uint     m_currentPass;

};