#include "precompiled.h"

#include <rgde/render/light.h>
#include <rgde/render/light_manager.h>

namespace render
{
	//base_light
	base_light::base_light(const std::string &name, type_t tipe)
	{
		m_name     = name;
		m_enabled = true;
		m_type    = tipe;
		//TheLightManager::get().addLight(this);
	}

	base_light::~base_light()
	{
		//TheLightManager::get().removeLight(this);
	}

	base_light::type_t base_light::type() const
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

	void base_light::setDirection(const math::vec3f& direction)
	{
		lookat(position(), position() + direction, up());
	}

	bool base_light::isEnabled() const
	{
		return m_enabled;
	}

	void base_light::setEnabled(bool bEnabled)
	{
		m_enabled = bEnabled;
	}

	//PointLight
	PointLight::PointLight(const std::string &name)
		: base_light(name, base_light::Point)
	{
		m_range                = 0.0f;
		m_fConstantAttenuation  = 1.0f;
		m_fLinearAttenuation    = 0.0f;
		m_fQuadraticAttenuation = 0.0f;
	}

	PointLight::~PointLight()
	{
	}

	float PointLight::range() const
	{
		return m_range;
	}

	void PointLight::range(float fRange)
	{
		m_range = fRange;
	}

	float PointLight::constant_attenuation() const
	{
		return m_fConstantAttenuation;
	}

	void PointLight::constant_attenuation(float value)
	{
		m_fConstantAttenuation = value;
	}

	float PointLight::linear_attenuation() const
	{
		return m_fLinearAttenuation;
	}

	void PointLight::linear_attenuation(float value)
	{
		m_fLinearAttenuation = value;
	}

	float PointLight::quadratic_attenuation() const
	{
		return m_fQuadraticAttenuation;
	}

	void PointLight::quadratic_attenuation(float value)
	{
		m_fQuadraticAttenuation = value;
	}

	//SpotLight
	SpotLight::SpotLight(const std::string &name)
		: base_light(name, base_light::Spot)
	{
		m_range   = 0.0f;
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

	float SpotLight::range() const
	{
		return m_range;
	}

	void SpotLight::range(float fRange)
	{
		m_range = fRange;
	}

	float SpotLight::constant_attenuation() const
	{
		return m_fConstantAttenuation;
	}

	void SpotLight::constant_attenuation(float value)
	{
		m_fConstantAttenuation = value;
	}

	float SpotLight::linear_attenuation() const
	{
		return m_fLinearAttenuation;
	}

	void SpotLight::linear_attenuation(float value)
	{
		m_fLinearAttenuation = value;
	}

	float SpotLight::quadratic_attenuation() const
	{
		return m_fQuadraticAttenuation;
	}

	void SpotLight::quadratic_attenuation(float value)
	{
		m_fQuadraticAttenuation = value;
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