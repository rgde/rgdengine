#include "precompiled.h"

#include "render/renderLight.h"
#include "render/renderLightManager.h"

namespace render
{
	//ILight
	ILight::ILight(const std::string &name, EType eType)
	{
		m_name     = name;
		m_bEnabled = true;
		m_eType    = eType;
		//TheLightManager::Get().addLight(this);
	}

	ILight::~ILight()
	{
		//TheLightManager::Get().removeLight(this);
	}

	ILight::EType ILight::getType() const
	{
		return m_eType;
	}

	const math::Color& ILight::getDiffuse() const
	{
		return m_diffuse;
	}

	void ILight::setDiffuse(const math::Color& diffuse)
	{
		m_diffuse = diffuse;
	}

	const math::Color& ILight::getSpecular() const
	{
		return m_specular;
	}

	void ILight::setSpecular(const math::Color& specular)
	{
		m_specular = specular;
	}

	const math::Color& ILight::getAmbient() const
	{
		return m_ambient;
	}

	void ILight::setAmbient(const math::Color& ambient)
	{
		m_ambient = ambient;
	}

	void ILight::setDirection(const math::Vec3f& direction)
	{
		math::Vec3f position= getPosition();
		lookAt(position, position + direction, getUp());
	}

	bool ILight::isEnabled() const
	{
		return m_bEnabled;
	}

	void ILight::setEnabled(bool bEnabled)
	{
		m_bEnabled = bEnabled;
	}

	//CPointLight
	CPointLight::CPointLight(const std::string &strName)
		: ILight(strName, ILight::Point)
	{
		m_fRange                = 0.0f;
		m_fConstantAttenuation  = 1.0f;
		m_fLinearAttenuation    = 0.0f;
		m_fQuadraticAttenuation = 0.0f;
	}

	CPointLight::~CPointLight()
	{
	}

	float CPointLight::getRange() const
	{
		return m_fRange;
	}

	void CPointLight::setRange(float fRange)
	{
		m_fRange = fRange;
	}

	float CPointLight::getConstantAttenuation() const
	{
		return m_fConstantAttenuation;
	}

	void CPointLight::setConstantAttenuation(float fConstantAttenuation)
	{
		m_fConstantAttenuation = fConstantAttenuation;
	}

	float CPointLight::getLinearAttenuation() const
	{
		return m_fLinearAttenuation;
	}

	void CPointLight::setLinearAttenuation(float fLinearAttenuation)
	{
		m_fLinearAttenuation = fLinearAttenuation;
	}

	float CPointLight::getQuadraticAttenuation() const
	{
		return m_fQuadraticAttenuation;
	}

	void CPointLight::setQuadraticAttenuation(float fQuadraticAttenuation)
	{
		m_fQuadraticAttenuation = fQuadraticAttenuation;
	}

	//CSpotLight
	CSpotLight::CSpotLight(const std::string &strName)
		: ILight(strName, ILight::Spot)
	{
		m_fRange   = 0.0f;
		m_fConstantAttenuation  = 1.0f;
		m_fLinearAttenuation    = 0.0f;
		m_fQuadraticAttenuation = 0.0f;
		m_fFalloff = 0.0f;
		m_fTheta   = 0.0f;
		m_fPhi     = 0.0f;
	}

	CSpotLight::~CSpotLight()
	{
	}

	float CSpotLight::getRange() const
	{
		return m_fRange;
	}

	void CSpotLight::setRange(float fRange)
	{
		m_fRange = fRange;
	}

	float CSpotLight::getConstantAttenuation() const
	{
		return m_fConstantAttenuation;
	}

	void CSpotLight::setConstantAttenuation(float fConstantAttenuation)
	{
		m_fConstantAttenuation = fConstantAttenuation;
	}

	float CSpotLight::getLinearAttenuation() const
	{
		return m_fLinearAttenuation;
	}

	void CSpotLight::setLinearAttenuation(float fLinearAttenuation)
	{
		m_fLinearAttenuation = fLinearAttenuation;
	}

	float CSpotLight::getQuadraticAttenuation() const
	{
		return m_fQuadraticAttenuation;
	}

	void CSpotLight::setQuadraticAttenuation(float fQuadraticAttenuation)
	{
		m_fQuadraticAttenuation = fQuadraticAttenuation;
	}

	float CSpotLight::getFalloff() const
	{
		return m_fFalloff;
	}

	void CSpotLight::setFalloff(float fFalloff)
	{
		m_fFalloff = fFalloff;
	}

	float CSpotLight::getTheta() const
	{
		return m_fTheta;
	}

	void CSpotLight::setTheta(float fTheta)
	{
		m_fTheta = fTheta;
	}

	float CSpotLight::getPhi() const
	{
		return m_fPhi;
	}

	void CSpotLight::setPhi(float fPhi)
	{
		m_fPhi = fPhi;
	}

	//CDirectionalLight
	CDirectionalLight::CDirectionalLight(const std::string &strName)
		: ILight(strName, ILight::Directional)
	{
	}

	CDirectionalLight::~CDirectionalLight()
	{
	}
}