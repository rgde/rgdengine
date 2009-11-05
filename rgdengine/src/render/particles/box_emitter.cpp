#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/emitter.h>
#include <rgde/render/particles/box_emitter.h>
#include <rgde/render/particles/particle.h>


namespace particles
{

	//-----------------------------------------------------------------------------------
	box_emitter::box_emitter() : base_emitter(emitter::Box)
	{
		m_name = "box_emitter";

		m_box_size.add_key(1, math::vec3f(3, 3, 3) );

		// public properties:
		//REGISTER_PROPERTY(BoxSize, math::Vec3Interp)
		//REGISTER_PROPERTY(BoxSizeSpread, math::Vec3Interp)
		//REGISTER_PROPERTY(Direction, math::Vec3Interp)
		//REGISTER_PROPERTY(DirectionSpread, math::Vec3Interp)

		//addProperty(new property<math::Vec3Interp>(m_box_size,			"BoxSize",			"Vec3Interp"));
		//addProperty(new property<math::Vec3Interp>(m_BoxSizeSpread,	"BoxSizeSpread",	"Vec3Interp"));
		//addProperty(new property<math::Vec3Interp>(m_Direction,		"Direction",		"Vec3Interp"));
		//addProperty(new property<math::Vec3Interp>(m_DirectionSpread,	"DirectionSpread",	"Vec3Interp"));
	}

	//-----------------------------------------------------------------------------------
	box_emitter::~box_emitter()
	{
	}

	//-----------------------------------------------------------------------------------
	void box_emitter::debug_draw()
	{
		base_emitter::debug_draw();

		math::vec3f size = m_box_size(m_fTimeNormalaized);
		math::vec3f size_rand = m_BoxSizeSpread(m_fTimeNormalaized);

		math::vec3f direction = m_Direction(m_fTimeNormalaized);
		math::vec3f direction_rand = m_DirectionSpread(m_fTimeNormalaized);

		math::matrix44f m = getTransform().get_full_tm();
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

		math::vec3f dir = m_Direction.getValue(m_fTimeNormalaized)
			+ (m_Rand()* 2.0f - 1.0f) * m_DirectionSpread.getValue(m_fTimeNormalaized);

		float velocity = m_velocity.getValue(m_fTimeNormalaized) 
			+ m_Rand() * m_PVelSpread.getValue(m_fTimeNormalaized);


		math::vec3f size = m_box_size.getValue(m_fTimeNormalaized)
			+ (m_Rand()* 2.0f - 1.0f) * m_BoxSizeSpread.getValue(m_fTimeNormalaized);

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
			<< (m_BoxSizeSpread)
			<< (m_Direction)
			<< (m_DirectionSpread);
	}

	//-----------------------------------------------------------------------------------
	void box_emitter::from_stream(io::read_stream& rf)
	{
		base_emitter::from_stream(rf);

		rf  >> (m_box_size)
			>> (m_BoxSizeSpread)
			>> (m_Direction)
			>> (m_DirectionSpread);
	}

}