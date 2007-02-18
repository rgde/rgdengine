//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 01.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pass.h"

CPass::CPass()
{
}

CPass::CPass(const CPass& pass)
{
	m_text = pass.getText();
}

CPass::~CPass()
{
}

void CPass::addText(const std::string& text)
{
	m_text += text;
}

void CPass::reset()
{
	m_text = "";
}

const std::string& CPass::getText() const
{
	return m_text;
}