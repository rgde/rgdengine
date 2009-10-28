/**--------------------------------------------------------------------------------------

									RGDE Engine

-----------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:	dxprg@mail.ru

Created: 7.06.2005
---------------------------------------------------------------------------------------*/
#pragma once

#include <rgde/io/io.h>

#include <rgde/math/transform.h>


namespace particles
{

	class emitter : public io::serialized_object, public core::meta_class
	{
	public:
		enum Type {	Static, Spherical, Box };

		emitter(Type _type) : m_type(_type), core::meta_class("ParticleEmitter") 
		{
			// public properties:
			//REGISTER_PROPERTY(Transform, math::Frame)
		}

		virtual void reset() = 0;
		virtual void update (float dt) = 0;

		virtual void render() = 0;
		virtual void debugDraw() = 0;

	public:
		virtual void setFade (bool bFade) = 0;

		// Акксессоры получения
		inline math::Frame& getTransform() { return m_Transform; }
		inline Type getEmitterType() const { return m_type; }

	protected:
		virtual void toStream(io::IWriteStream& wf) const;
		virtual void fromStream(io::IReadStream& rf);

	protected:
		const Type		m_type;					// Тип эммитера
		math::Frame	m_Transform;				// Трансформация
	};

}