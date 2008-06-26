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

	class Emitter : public io::ISerializedObject, public core::XmlClass
	{
	public:
		enum Type {	Maya, Spherical, Box };

		Emitter(Type _type) : m_eType(_type), core::XmlClass("ParticleEmitter") 
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
		inline Type getEmitterType() const { return m_eType; }

	protected:
		virtual void toStream(io::IWriteStream& wf) const;
		virtual void fromStream(io::IReadStream& rf);

	protected:
		const Type		m_eType;					// Тип эммитера
		math::Frame	m_Transform;				// Трансформация
	};

}