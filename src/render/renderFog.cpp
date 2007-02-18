#include "precompiled.h"

#include "render/renderFog.h"

#include "base/xml_helpers.h"
#include "base/log.h"
#include "base/lexical_cast.h"

namespace render
{
	inline CFog::Type convertMode(const std::string &strMode)
	{
		std::string strModeLow	= strMode;

		base::Lower_case<std::string>(strModeLow);

		if (strModeLow == "exp")
			return CFog::Exp;
		else if (strModeLow == "exp2")
			return CFog::Exp2;
		else if (strModeLow == "linear")
			return CFog::Linear;
		else
			return CFog::None;
	}

	void CFog::loadFromXML(TiXmlElement *node)
	{
		base::read(m_color, node, "color");

		std::string strMode	= base::safeReadValue<std::string>(node, "mode", "None");
		setMode(convertMode(strMode));

		m_fStart = base::safeReadValue<float>(node, "start", 0.0f);
		m_fEnd = base::safeReadValue<float>(node, "end", 0.0f);
	}

	void CFog::loadFromXML(const std::string &strFileName)
	{
		TiXmlDocument fog;
		{
			io::ScopePathAdd p	("Fogs/");
			if (!base::loadXml(strFileName, fog))
			{
				base::lerr << "Can't load fog \"" << strFileName << "\".";
				return;
			}
		}

		TiXmlElement *root	= fog.FirstChildElement("fog");

		if (0 != root)
			loadFromXML(root);
	}

	void CFog::setMode(Type type)
	{
		m_type = type;
		m_bEnabled = m_type != None;
	}

	CFog::CFog()
		: m_fStart(0),
		  m_fEnd(0)
	{
		setMode(None);
	}
}