#include "precompiled.h"

#include <rgde/render/fog.h>

#include <rgde/base/xml_helpers.h>
#include <rgde/base/log.h>
#include <rgde/base/lexical_cast.h>

namespace render
{
	inline Fog::Type convertMode(const std::string &strMode)
	{
		std::string strModeLow	= strMode;

		base::lower_case<std::string>(strModeLow);

		if (strModeLow == "exp")
			return Fog::Exp;
		else if (strModeLow == "exp2")
			return Fog::Exp2;
		else if (strModeLow == "linear")
			return Fog::Linear;
		else
			return Fog::None;
	}

	void Fog::load_from_xml(TiXmlElement *node)
	{
		base::read(m_color, node, "color");

		std::string strMode	= base::safe_read<std::string>(node, "mode", "None");
		setMode(convertMode(strMode));

		m_fStart = base::safe_read<float>(node, "start", 0.0f);
		m_fEnd = base::safe_read<float>(node, "end", 0.0f);
	}

	void Fog::load_from_xml(const std::string &file_name)
	{
		TiXmlDocument fog;
		{
			io::path_add_scoped p	("Fogs/");
			if (!base::load_xml(file_name, fog))
			{
				base::lerr << "Can't load fog \"" << file_name << "\".";
				return;
			}
		}

		TiXmlElement *root	= fog.FirstChildElement("fog");

		if (0 != root)
			load_from_xml(root);
	}

	void Fog::setMode(Type type)
	{
		m_type = type;
		m_bEnabled = m_type != None;
	}

	Fog::Fog()
		: m_fStart(0),
		  m_fEnd(0)
	{
		setMode(None);
	}
}