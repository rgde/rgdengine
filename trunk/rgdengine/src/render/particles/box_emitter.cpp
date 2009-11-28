#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/box_emitter.h>
#include <rgde/render/particles/particle.h>


namespace particles
{

	//-----------------------------------------------------------------------------------
	box_emitter::box_emitter() 
		: base_emitter(base_emitter::box)		
	{
		m_name = "box_emitter";
		m_box_size.add_key(1, math::vec3f(3, 3, 3) );
	}

	//-----------------------------------------------------------------------------------
	box_emitter::~box_emitter()
	{
	}

	//-----------------------------------------------------------------------------------
	void box_emitter::debug_draw()
	{
		base_emitter::debug_draw();

		math::vec3f size = m_box_size(m_normalized_time);
		math::vec3f size_rand = m_box_size_spread(m_normalized_time);

		math::vec3f direction = m_direction(m_normalized_time);
		math::vec3f direction_rand = m_direction_spread(m_normalized_time);

		math::matrix44f m = get_transform()->get_full_tm();
		render::lines3d& line_manager = render::render_device::get().get_lines3d();
		line_manager.add_box( m, (math::vec3f)(size + size_rand), math::Color(0, 255, 0, 255) );
		line_manager.add_box( m, (math::vec3f)(size - size_rand), math::Color(0, 255, 0, 255) );

		line_manager.add_box( m, size, math::Color(0, 255, 0, 255) );

		line_manager.add_arrow( m, direction, math::Color(0, 255, 0, 255) );
	}

	//-----------------------------------------------------------------------------------
	void box_emitter::get_particle(particle& p)
	{
		base_emitter::get_particle(p);

		math::vec3f dir = m_direction.get_value(m_normalized_time)
			+ (m_Rand()* 2.0f - 1.0f) * m_direction_spread.get_value(m_normalized_time);

		float velocity = m_velocity.get_value(m_normalized_time) 
			+ m_Rand() * m_PVelSpread.get_value(m_normalized_time);


		math::vec3f size = m_box_size.get_value(m_normalized_time)
			+ (m_Rand()* 2.0f - 1.0f) * m_box_size_spread.get_value(m_normalized_time);

		float x = (m_Rand()* 2.0f - 1.0f) * size[0];
		float y = (m_Rand()* 2.0f - 1.0f) * size[1];
		float z = (m_Rand()* 2.0f - 1.0f) * size[2];

		math::vec3f pos(x, y, z);

		p.initial_pos = pos;
		//math::normalize( pos );
		//pos.normalize();

		p.vel = velocity * dir;
	}

	//-----------------------------------------------------------------------------------
	void box_emitter::to_stream(io::write_stream& wf) const
	{
		base_emitter::to_stream(wf);

		wf	<< (m_box_size)
			<< (m_box_size_spread)
			<< (m_direction)
			<< (m_direction_spread);
	}

	//-----------------------------------------------------------------------------------
	void box_emitter::from_stream(io::read_stream& rf)
	{
		base_emitter::from_stream(rf);

		rf  >> (m_box_size)
			>> (m_box_size_spread)
			>> (m_direction)
			>> (m_direction_spread);
	}

}