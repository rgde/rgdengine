#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/spherical_emitter.h>
#include <rgde/render/particles/particle.h>

#include <rgde/math/types3d.h>


namespace particles
{

	//-----------------------------------------------------------------------------------
	spherical_emitter::spherical_emitter() 
		: base_emitter(base_emitter::spherical)
	{
		m_Angle.add_key(1, 45.0f);
		m_Radius.add_key(0, 0);
		m_Radius.add_key(1.0f, 9.0f);
	}

	//-----------------------------------------------------------------------------------
	spherical_emitter::~spherical_emitter()
	{
	}

	//-----------------------------------------------------------------------------------
	void spherical_emitter::get_particle(particle& p)
	{
		base_emitter::get_particle(p);

		float radius = m_Radius.get_value(m_normalized_time)
			+ (m_Rand()* 2.0f - 1.0f) * m_RadiusSpread.get_value(m_normalized_time);
		float velocity = m_velocity.get_value(m_normalized_time) 
			+ m_Rand() * m_PVelSpread.get_value(m_normalized_time);

		float angle = m_Angle.get_value(m_normalized_time);

		float a = 90 - math::Math::unitRandom()/*m_Rand()*/* angle;
		float b = math::Math::unitRandom()/*m_Rand()*/* 360;
		
		static const float angl_to_rad = 3.1415f / 180.0f;
		
		float r1 = cosf(a * angl_to_rad) * radius;

		float z = cosf(b * angl_to_rad) * r1;
		float y = sinf(a * angl_to_rad) * radius;
		float x = sinf(b * angl_to_rad) * r1;

		math::vec3f pos(x, y, z);
		p.initial_pos = pos;
		math::normalize( pos );
		p.vel = pos * velocity;
	}

	//-----------------------------------------------------------------------------------
	void spherical_emitter::debug_draw()
	{
		base_emitter::debug_draw();

		render::lines3d& line_manager = render::render_device::get().get_lines3d();

		float rad = m_Radius.get_value(m_normalized_time);// + 
		float r_rand = m_RadiusSpread.get_value(m_normalized_time);
		int angle = (int)(m_Angle.get_value(m_normalized_time));

		math::matrix44f m = get_transform()->get_full_tm();

		line_manager.add_sphere( m , rad, angle );
		if( r_rand != 0 )
		{
			line_manager.add_sphere( m , rad-r_rand, angle );
			line_manager.add_sphere( m , rad+r_rand, angle );
		}
	}

	//-----------------------------------------------------------------------------------
	void spherical_emitter::to_stream(io::write_stream& wf) const
	{
		base_emitter::to_stream(wf);

		wf	<< m_Radius
			<< m_RadiusSpread
			<< m_Angle;	
	}

	//-----------------------------------------------------------------------------------
	void spherical_emitter::from_stream(io::read_stream& rf)
	{
		base_emitter::from_stream(rf);

		rf  >> m_Radius
			>> m_RadiusSpread
			>> m_Angle;	
	}
}