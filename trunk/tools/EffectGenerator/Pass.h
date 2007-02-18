//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 01.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

class CPass
{
public:

	CPass();
	CPass(const CPass& pass);
	~CPass();

	void addText(const std::string& text);

	void reset();

	const std::string& getText() const;

private:

	std::string m_text;

};