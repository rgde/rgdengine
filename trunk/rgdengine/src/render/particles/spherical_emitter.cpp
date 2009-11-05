#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/spherical_emitter.h>
#include <rgde/render/particles/particle.h>

#include <rgde/math/types3d.h>


namespace particles
{

	//-----------------------------------------------------------------------------------
	spherical_emitter::spherical_emitter() : base_emitter(emitter::Spherical)
	{
		m_Angle.add_key(1, 45.0f);
		m_Radius.add_key(0, 0);
		m_Radius.add_key(1.0f, 9.0f);	

		//addProperty(new property<math::FloatInterp>(m_Radius,			"Radius",	"FloatInterp"));
		//addProperty(new property<math::FloatInterp>(m_RadiusSpread,	"RadiusSpread",	"FloatInterp"));
		//addProperty(new property<math::FloatInterp>(m_Angle,			"Angle",	"FloatInterp"));

		// public properties:
		//REGISTER_PROPERTY(Radius,		math::FloatInterp)
		//REGISTER_PROPERTY(RadiusSpread,	math::FloatInterp)
		//REGISTER_PROPERTY(Angle,		math::FloatInterp)
	}

	//-----------------------------------------------------------------------------------
	spherical_emitter::~spherical_emitter()
	{
	}

	//-----------------------------------------------------------------------------------
	void spherical_emitter::get_particle(particle& p)
	{
		base_emitter::get_particle(p);

		float radius = m_Radius.getValue(m_fTimeNormalaized)
			+ (m_Rand()* 2.0f - 1.0f) * m_RadiusSpread.getValue(m_fTimeNormalaized);
		float velocity = m_PVelocity.getValue(m_fTimeNormalaized) 
			+ m_Rand() * m_PVelSpread.getValue(m_fTimeNormalaized);

		float angle = m_Angle.getValue(m_fTimeNormalaized);

		float a = 90 - math::Math::unitRandom()/*m_Rand()*/* angle;
		float b = math::Math::unitRandom()/*m_Rand()*/* 360;
		
		static const float angl_to_rad = 3.1415f / 180.0f;
		
		float r1 = cosf(a * angl_to_rad) * radius;

		float z = cosf(b * angl_to_rad) * r1;
		float y = sinf(a * angl_to_rad) * radius;
		float x = sinf(b * angl_to_rad) * r1;

		math::Vec3f pos(x, y, z);
		p.initial_pos = pos;
		math::normalize( pos );
		p.vel = pos * velocity;
	}

	//-----------------------------------------------------------------------------------
	void spherical_emitter::debug_draw()
	{
		base_emitter::debug_draw();

		render::lines3d& line_manager = render::render_device::get().get_lines3d();

		float rad = m_Radius.getValue(m_fTimeNormalaized);// + 
		float r_rand = m_RadiusSpread.getValue(m_fTimeNormalaized);
		int angle = (int)(m_Angle.getValue(m_fTimeNormalaized));

		math::Matrix44f m = getTransform().get_full_tm();

		line_manager.add_sphere( m , rad, angle );
		if( r_rand != 0 )
		{
			line_manager.add_sphere( m , rad-r_rand, angle );
			line_manager.add_sphere( m , rad+r_rand, angle );
		}
	}

	//-----------------------------------------------------------------------------------
	void spherical_emitter::toStream(io::write_stream& wf) const
	{
		base_emitter::toStream(wf);

		wf	<< m_Radius
			<< m_RadiusSpread
			<< m_Angle;	
	}

	//-----------------------------------------------------------------------------------
	void spherical_emitter::fromStream(io::read_stream& rf)
	{
		base_emitter::fromStream(rf);

		rf  >> m_Radius
			>> m_RadiusSpread
			>> m_Angle;	
	}
}