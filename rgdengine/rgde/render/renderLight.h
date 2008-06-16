#pragma once

#include <rgde/math/mathTransform.h>

namespace render
{
	//ILight
	class ILight : public math::Frame
	{
	public:
		enum EType 
		{
			None            = 0,
			Point			= 1,
			Spot			= 2,
			Directional		= 3
		};

		ILight(const std::string& name, EType eType);
		virtual ~ILight();

		EType getType() const;

		const math::Color& getDiffuse() const;
		void               setDiffuse(const math::Color& diffuse);

		const math::Color& getSpecular() const;
		void               setSpecular(const math::Color& specular);

		const math::Color& getAmbient() const;
		void               setAmbient(const math::Color& ambient);

		void setDirection(const math::Vec3f& direction);

		bool isEnabled() const;
		void setEnabled(bool bEnabled);

	protected:

		math::Color m_diffuse; //Diffuse color of light
		math::Color m_specular;//Specular color of light
		math::Color m_ambient; //Ambient color of light

		bool m_bEnabled;       //Is light enabled

		EType m_eType;         //Light type
	};

	typedef boost::intrusive_ptr<ILight> PLight;

	//PointLight
	class PointLight : public ILight
	{
	public:

		PointLight(const std::string& name);
		~PointLight();

		float getRange() const;
		void  setRange(float fRange);

		float getConstantAttenuation() const;
		void  setConstantAttenuation(float fConstantAttenuation);

		float getLinearAttenuation() const;
		void  setLinearAttenuation(float fLinearAttenuation);

		float getQuadraticAttenuation() const;
		void  setQuadraticAttenuation(float fQuadraticAttenuation);

	protected:
	
		float m_fRange;               //Cutoff range 
		float m_fConstantAttenuation; //Constant attenuation
		float m_fLinearAttenuation;   //Linear attenuation 
		float m_fQuadraticAttenuation;//Quadratic attenuation
   
	};

	//SpotLight
	class SpotLight : public ILight
	{
	public:

		SpotLight(const std::string& name);
		~SpotLight();

		float getRange() const;
		void  setRange(float fRange);

		float getConstantAttenuation() const;
		void  setConstantAttenuation(float fConstantAttenuation);

		float getLinearAttenuation() const;
		void  setLinearAttenuation(float fLinearAttenuation);

		float getQuadraticAttenuation() const;
		void  setQuadraticAttenuation(float fQuadraticAttenuation);

		float getFalloff() const;
		void  setFalloff(float fFalloff);

		float getTheta() const;
		void  setTheta(float fTheta);

		float getPhi() const;
		void  setPhi(float fPhi);

	protected:

		float m_fRange;               //Cutoff range 
		float m_fConstantAttenuation; //Constant attenuation
		float m_fLinearAttenuation;   //Linear attenuation 
		float m_fQuadraticAttenuation;//Quadratic attenuation
		float m_fFalloff;             //Falloff 
		float m_fTheta;               //Inner angle of spotlight cone
		float m_fPhi;                 //Outer angle of spotlight cone
	};

	//DirectionalLight
	class DirectionalLight : public ILight
	{
	public:

		DirectionalLight(const std::string& name);
		~DirectionalLight();

	protected:
	
	};
}