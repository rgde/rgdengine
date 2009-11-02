#include "precompiled.h"

#include <rgde/render/light.h>
#include <rgde/render/light_manager.h>

namespace render
{
	//base_light
	base_light::base_light(const std::string &name, type eType)
	{
		m_name     = name;
		m_bEnabled = true;
		m_type    = eType;
		//TheLightManager::get().addLight(this);
	}

	base_light::~base_light()
	{
		//TheLightManager::get().removeLight(this);
	}

	base_light::type base_light::get_type() const
	{
		return m_type;
	}

	const math::Color& base_light::getDiffuse() const
	{
		return m_diffuse;
	}

	void base_light::setDiffuse(const math::Color& diffuse)
	{
		m_diffuse = diffuse;
	}

	const math::Color& base_light::getSpecular() const
	{
		return m_specular;
	}

	void base_light::setSpecular(const math::Color& specular)
	{
		m_specular = specular;
	}

	const math::Color& base_light::getAmbient() const
	{
		return m_ambient;
	}

	void base_light::setAmbient(const math::Color& ambient)
	{
		m_ambient = ambient;
	}

	void base_light::setDirection(const math::Vec3f& direction)
	{
		math::Vec3f position= get_pos();
		look_at(position, position + direction, getUp());
	}

	bool base_light::isEnabled() const
	{
		return m_bEnabled;
	}

	void base_light::setEnabled(bool bEnabled)
	{
		m_bEnabled = bEnabled;
	}

	//PointLight
	PointLight::PointLight(const std::string &name)
		: base_light(name, base_light::Point)
	{
		m_fRange                = 0.0f;
		m_fConstantAttenuation  = 1.0f;
		m_fLinearAttenuation    = 0.0f;
		m_fQuadraticAttenuation = 0.0f;
	}

	PointLight::~PointLight()
	{
	}

	float PointLight::getRange() const
	{
		return m_fRange;
	}

	void PointLight::setRange(float fRange)
	{
		m_fRange = fRange;
	}

	float PointLight::getConstantAttenuation() const
	{
		return m_fConstantAttenuation;
	}

	void PointLight::setConstantAttenuation(float fConstantAttenuation)
	{
		m_fConstantAttenuation = fConstantAttenuation;
	}

	float PointLight::getLinearAttenuation() const
	{
		return m_fLinearAttenuation;
	}

	void PointLight::setLinearAttenuation(float fLinearAttenuation)
	{
		m_fLinearAttenuation = fLinearAttenuation;
	}

	float PointLight::getQuadraticAttenuation() const
	{
		return m_fQuadraticAttenuation;
	}

	void PointLight::setQuadraticAttenuation(float fQuadraticAttenuation)
	{
		m_fQuadraticAttenuation = fQuadraticAttenuation;
	}

	//SpotLight
	SpotLight::SpotLight(const std::string &name)
		: base_light(name, base_light::Spot)
	{
		m_fRange   = 0.0f;
		m_fConstantAttenuation  = 1.0f;
		m_fLinearAttenuation    = 0.0f;
		m_fQuadraticAttenuation = 0.0f;
		m_fFalloff = 0.0f;
		m_fTheta   = 0.0f;
		m_fPhi     = 0.0f;
	}

	SpotLight::~SpotLight()
	{
	}

	float SpotLight::getRange() const
	{
		return m_fRange;
	}

	void SpotLight::setRange(float fRange)
	{
		m_fRange = fRange;
	}

	float SpotLight::getConstantAttenuation() const
	{
		return m_fConstantAttenuation;
	}

	void SpotLight::setConstantAttenuation(float fConstantAttenuation)
	{
		m_fConstantAttenuation = fConstantAttenuation;
	}

	float SpotLight::getLinearAttenuation() const
	{
		return m_fLinearAttenuation;
	}

	void SpotLight::setLinearAttenuation(float fLinearAttenuation)
	{
		m_fLinearAttenuation = fLinearAttenuation;
	}

	float SpotLight::getQuadraticAttenuation() const
	{
		return m_fQuadraticAttenuation;
	}

	void SpotLight::setQuadraticAttenuation(float fQuadraticAttenuation)
	{
		m_fQuadraticAttenuation = fQuadraticAttenuation;
	}

	float SpotLight::getFalloff() const
	{
		return m_fFalloff;
	}

	void SpotLight::setFalloff(float fFalloff)
	{
		m_fFalloff = fFalloff;
	}

	float SpotLight::getTheta() const
	{
		return m_fTheta;
	}

	void SpotLight::setTheta(float fTheta)
	{
		m_fTheta = fTheta;
	}

	float SpotLight::getPhi() const
	{
		return m_fPhi;
	}

	void SpotLight::setPhi(float fPhi)
	{
		m_fPhi = fPhi;
	}

	//DirectionalLight
	DirectionalLight::DirectionalLight(const std::string &name)
		: base_light(name, base_light::Directional)
	{
	}

	DirectionalLight::~DirectionalLight()
	{
	}
}