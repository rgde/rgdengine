#pragma once

#include <rgde/io/io.h>
#include <rgde/base/lexical_cast.h>
#include <rgde/math/transform.h>

namespace base
{
	inline bool load_xml(const std::string& filename, TiXmlDocument& document)
	{
		std::vector<byte> data;
		{
			io::readstream_ptr in = io::file_system::get().find(filename);

			if(!in)
				return false;

			io::stream_to_vector(data, in);
		}

		document.Parse((const char*)&(data[0]));

		return true;
	}

	template<typename T> 
	inline T safe_read(TiXmlHandle hBaseNode,
						   const std::string& strParameterName,
						   const T& defaultValue) 
	{ 
		try {
			TiXmlText* pText = hBaseNode.FirstChildElement(strParameterName).FirstChild().Text();
			if (0 == pText) return defaultValue;

			const char* str = pText->Value();
			if (0 == str) return defaultValue;

			std::string strValue = str;
			return base::lexical_cast<T>(strValue); 
		} catch(...)
		{
			base::lwrn<<"Can't get value of param \""<<strParameterName<<"\", using default value : \""<<defaultValue<<"\"";
			return defaultValue;
		}
	}

	template<typename T> 
	inline T safe_read(TiXmlHandle hBaseNode,
						   const T& defaultValue) 
	{ 
		try {
			std::string strValue = hBaseNode.FirstChild().Text()->Value();
			return base::lexical_cast<T>(strValue); 
		} catch(...)
		{
			base::lwrn<<"Can't get value of param, using default value : \""<<defaultValue<<"\"";
			return defaultValue;
		}
	}

	template<typename T> 
	inline T safe_read(TiXmlElement* pElement,
						   const std::string& strParameterName,
						   const T& defaultValue) 
	{ 
		try {
			std::string strValue = pElement->FirstChildElement(strParameterName)->FirstChild()->ToText()->Value();
			return base::lexical_cast<T>(strValue); 
		} catch(...)
		{
			base::lwrn<<"Can't get value of param \""<<strParameterName<<"\", using default value : \""<<defaultValue<<"\"";
			return defaultValue;
		}
	}

	template<typename T> 
	inline T safe_read(TiXmlElement* pElement,
						   const T& defaultValue) 
	{ 
		try {
			std::string strValue = pElement->FirstChild()->ToText()->Value();
			return base::lexical_cast<T>(strValue); 
		} catch(...)
		{
			base::lwrn<<"Can't get value of param, using default value : \""<<defaultValue<<"\"";
			return defaultValue;
		}
	}

	template<typename T> 
	inline T safe_read_attr(TiXmlHandle hBaseNode,
									const std::string& strParameterName,
									const std::string& strAttributeName,
									const T& defaultValue)
	{ 
		try {
			TiXmlHandle h = hBaseNode.FirstChildElement(strParameterName);
			TiXmlElement* pEl = h.Element();
			if (0 == pEl) return defaultValue;

			const char* value = pEl->Attribute(strAttributeName.c_str());
			if (0==value) return defaultValue;
			std::string strValue(value);
			return base::lexical_cast<T>(strValue); 
		} catch(...)
		{
			base::lwrn<<"Can't get value of attribute \""<<strAttributeName<<"\" of parameter \""<<strParameterName<<"\", using default value : \""<<defaultValue<<"\"";
			return defaultValue;
		}
	}

	template<typename T> 
	inline T safe_read_attr(TiXmlElement* pElement,
									const std::string& strParameterName,
									const std::string& strAttributeName,
									const T& defaultValue)
	{ 
		try {
			std::string strValue = pElement->FirstChildElement(strParameterName)->Attribute(strAttributeName);
			return base::lexical_cast<T>(strValue); 
		} catch(...)
		{
			base::lwrn<<"Can't get value of attribute \""<<strAttributeName<<"\" of parameter \""<<strParameterName<<"\", using default value : \""<<defaultValue<<"\"";
			return defaultValue;
		}
	}

	inline void read(math::Color& color, TiXmlElement* pElement, const std::string& strParameterName)
	{
		char r = (char)safe_read_attr<int>(pElement, strParameterName, "r", 0);
		char g = (char)safe_read_attr<int>(pElement, strParameterName, "g", 0);
		char b = (char)safe_read_attr<int>(pElement, strParameterName, "b", 0);
		char a = (char)safe_read_attr<int>(pElement, strParameterName, "a", 0);

		color = math::Color(r, g, b, a);
	}

	inline void read(math::vec4f& v, TiXmlElement* node)
	{
		double x = 0;
		node->Attribute("x", &x);

		double y = 0;
		node->Attribute("y", &y);

		double z = 0;
		node->Attribute("z", &z);

		double w = 0;
		node->Attribute("w", &w);

		v[0] = static_cast<float>(x);
		v[1] = static_cast<float>(y);
		v[2] = static_cast<float>(z);
		v[3] = static_cast<float>(w);
	}

	inline void read(math::vec3f& v, TiXmlElement* node)
	{
		double x = 0;
		node->Attribute("x", &x);

		double y = 0;
		node->Attribute("y", &y);

		double z = 0;
		node->Attribute("z", &z);

		v[0] = static_cast<float>(x);
		v[1] = static_cast<float>(y);
		v[2] = static_cast<float>(z);
	}

	inline void read(math::vec2f& v, TiXmlElement* node)
	{
		double x = 0;
		node->Attribute("u", &x);

		double y = 0;
		node->Attribute("v", &y);

		v[0] = static_cast<float>(x);
		v[1] = static_cast<float>(y);
	}

	template<typename T> 
	inline T safe_read_node_name(TiXmlNode* node,	const T& defaultValue) 
	{ 
		try {
			std::string strValue = node->Value();
			return base::lexical_cast<T>(strValue); 
		} catch(...)
		{
			base::lwrn<<"Can't get value of node, using default value : \""<<defaultValue<<"\"";
			return defaultValue;
		}
	}

	template<typename T> 
	inline T safe_read_attr(TiXmlElement* pElement,	const char* strAttributeName,const T& defaultValue)
	{ 
		try {
			std::string strValue = pElement->Attribute(strAttributeName);
			return base::lexical_cast<T>(strValue); 
		} catch(...)
		{
			//base::lwrn << "Can't get value of attribute \""<<strAttributeName<<"\" of parameter, using default value : \""<<defaultValue<<"\"";
			return defaultValue;
		}
	}

	inline void read_color(math::Color &color, TiXmlElement *elem, float alpha = 1.0f)
	{
		if (NULL == elem)
			return;
		TiXmlElement *pColorElem= elem->FirstChildElement("color");
		if (NULL == pColorElem)
			return;

		double r= 1.0, g = 1.0, b = 1.0;

		pColorElem->Attribute("red", &r);
		pColorElem->Attribute("green", &g);
		pColorElem->Attribute("blue", &b);

		color.set(r, g, b, (double)alpha);
	}

	inline void read_light_color(math::Color& color, TiXmlElement* pElement, const std::string& strParameterName)
	{
		char r = (char)(safe_read_attr<float>(pElement, strParameterName, "red", 0)*255.0f);
		char g = (char)(safe_read_attr<float>(pElement, strParameterName, "green", 0)*255.0f);
		char b = (char)(safe_read_attr<float>(pElement, strParameterName, "blue", 0)*255.0f);
		char a = (char)(safe_read_attr<float>(pElement, strParameterName, "alpha", 0)*255.0f);

		color = math::Color(r, g, b, a);
	}
}