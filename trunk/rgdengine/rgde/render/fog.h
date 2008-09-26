#pragma once

#include <rgde/math/types3d.h>

namespace render
{
	class Fog
	{
	public:
		enum Type
		{
			None,
			Exp,
			Exp2,
			Linear
		};

		Fog();

		void setMode(Type type);
		void loadFromXML(const std::string& file_name);
		void loadFromXML(TiXmlElement* node);

	private:
		bool        m_bEnabled;
		math::Color m_color;
		Type		m_type;
		float       m_fStart;
		float       m_fEnd;
	};
}