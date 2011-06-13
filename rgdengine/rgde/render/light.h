#pragma once

#include <rgde/math/transform.h>

namespace render
{
	//base_light
	class base_light : public math::frame
	{
	public:
		enum type_t
		{
			None            = 0,
			Point			= 1,
			Spot			= 2,
			Directional		= 3
		};

		base_light(const std::string& name, type_t tipe);
		virtual ~base_light();

		type_t type() const;

		const math::Color& getDiffuse() const;
		void               setDiffuse(const math::Color& diffuse);

		const math::Color& getSpecular() const;
		void               setSpecular(const math::Color& specular);

		const math::Color& getAmbient() const;
		void               setAmbient(const math::Color& ambient);

		void setDirection(const math::vec3f& direction);

		bool isEnabled() const;
		void setEnabled(bool bEnabled);

	protected:
		math::Color m_diffuse; //Diffuse color of light
		math::Color m_specular;//Specular color of light
		math::Color m_ambient; //Ambient color of light

		bool m_enabled;       //Is light enabled

		type_t m_type;         //Light type
	};

	typedef boost::intrusive_ptr<base_light> PLight;

	//PointLight
	class PointLight : public base_light
	{
	public:
		PointLight(const std::string& name);
		~PointLight();

		float range() const;
		void  range(float fRange);

		float constant_attenuation() const;
		void  constant_attenuation(float value);

		float linear_attenuation() const;
		void  linear_attenuation(float value);

		float quadratic_attenuation() const;
		void  quadratic_attenuation(float value);

	protected:
	
		float m_range;               //Cutoff range 
		float m_fConstantAttenuation; //Constant attenuation
		float m_fLinearAttenuation;   //Linear attenuation 
		float m_fQuadraticAttenuation;//Quadratic attenuation
   
	};

	//SpotLight
	class SpotLight : public base_light
	{
	public:

		SpotLight(const std::string& name);
		~SpotLight();

		float range() const;
		void  range(float fRange);

		float constant_attenuation() const;
		void  constant_attenuation(float value);

		float linear_attenuation() const;
		void  linear_attenuation(float value);

		float quadratic_attenuation() const;
		void  quadratic_attenuation(float value);

		float getFalloff() const;
		void  setFalloff(float fFalloff);

		float getTheta() const;
		void  setTheta(float fTheta);

		float getPhi() const;
		void  setPhi(float fPhi);

	protected:

		float m_range;               //Cutoff range 
		float m_fConstantAttenuation; //Constant attenuation
		float m_fLinearAttenuation;   //Linear attenuation 
		float m_fQuadraticAttenuation;//Quadratic attenuation
		float m_fFalloff;             //Falloff 
		float m_fTheta;               //Inner angle of spotlight cone
		float m_fPhi;                 //Outer angle of spotlight cone
	};

	//DirectionalLight
	class DirectionalLight : public base_light
	{
	public:

		DirectionalLight(const std::string& name);
		~DirectionalLight();

	protected:
	
	};
}