#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/emitter.h>
#include <rgde/render/particles/box_emitter.h>
#include <rgde/render/particles/particle.h>


namespace particles
{

	//-----------------------------------------------------------------------------------
	BoxEmitter::BoxEmitter() : AbstractEmitter(Emitter::Box)
	{
		m_name = "BoxEmitter";

		m_BoxSize.addKey(1, math::Vec3f(3, 3, 3) );

		// public properties:
		//REGISTER_PROPERTY(BoxSize, math::Vec3Interp)
		//REGISTER_PROPERTY(BoxSizeSpread, math::Vec3Interp)
		//REGISTER_PROPERTY(Direction, math::Vec3Interp)
		//REGISTER_PROPERTY(DirectionSpread, math::Vec3Interp)

		//addProperty(new TProperty<math::Vec3Interp>(m_BoxSize,			"BoxSize",			"Vec3Interp"));
		//addProperty(new TProperty<math::Vec3Interp>(m_BoxSizeSpread,	"BoxSizeSpread",	"Vec3Interp"));
		//addProperty(new TProperty<math::Vec3Interp>(m_Direction,		"Direction",		"Vec3Interp"));
		//addProperty(new TProperty<math::Vec3Interp>(m_DirectionSpread,	"DirectionSpread",	"Vec3Interp"));
	}

	//-----------------------------------------------------------------------------------
	BoxEmitter::~BoxEmitter()
	{
	}

	//-----------------------------------------------------------------------------------
	void BoxEmitter::debugDraw()
	{
		AbstractEmitter::debugDraw();

		math::Vec3f size = m_BoxSize(m_fTimeNormalaized);
		math::Vec3f size_rand = m_BoxSizeSpread(m_fTimeNormalaized);

		math::Vec3f direction = m_Direction(m_fTimeNormalaized);
		math::Vec3f direction_rand = m_DirectionSpread(m_fTimeNormalaized);

		math::Matrix44f m = getTransform().getFullTransform();
		render::Line3dManager& line_manager = render::TheLine3dManager::Get();
		line_manager.addBox( m, (math::Vec3f)(size + size_rand), math::Color(0, 255, 0, 255) );
		line_manager.addBox( m, (math::Vec3f)(size - size_rand), math::Color(0, 255, 0, 255) );

		line_manager.addBox( m, size, math::Color(0, 255, 0, 255) );

		line_manager.addDirection( m, direction, math::Color(0, 255, 0, 255) );
	}

	//-----------------------------------------------------------------------------------
	void BoxEmitter::getParticle(Particle& p)
	{
		AbstractEmitter::getParticle(p);

		math::Vec3f dir = m_Direction.getValue(m_fTimeNormalaized)
			+ (m_Rand()* 2.0f - 1.0f) * m_DirectionSpread.getValue(m_fTimeNormalaized);

		float velocity = m_PVelocity.getValue(m_fTimeNormalaized) 
			+ m_Rand() * m_PVelSpread.getValue(m_fTimeNormalaized);


		math::Vec3f size = m_BoxSize.getValue(m_fTimeNormalaized)
			+ (m_Rand()* 2.0f - 1.0f) * m_BoxSizeSpread.getValue(m_fTimeNormalaized);

		float x = (m_Rand()* 2.0f - 1.0f) * size[0];
		float y = (m_Rand()* 2.0f - 1.0f) * size[1];
		float z = (m_Rand()* 2.0f - 1.0f) * size[2];

		math::Vec3f pos(x, y, z);

		p.initial_pos = pos;
		//math::normalize( pos );
		//pos.normalize();

		p.vel = velocity * dir;
	}

	//-----------------------------------------------------------------------------------
	void BoxEmitter::toStream(io::IWriteStream& wf) const
	{
		AbstractEmitter::toStream(wf);

		wf	<< (m_BoxSize)
			<< (m_BoxSizeSpread)
			<< (m_Direction)
			<< (m_DirectionSpread);
	}

	//-----------------------------------------------------------------------------------
	void BoxEmitter::fromStream(io::IReadStream& rf)
	{
		AbstractEmitter::fromStream(rf);

		rf  >> (m_BoxSize)
			>> (m_BoxSizeSpread)
			>> (m_Direction)
			>> (m_DirectionSpread);
	}

}