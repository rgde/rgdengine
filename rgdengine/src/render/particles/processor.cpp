#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/processor.h>
#include <rgde/render/particles/tank.h>
#include <rgde/render/particles/abstract_emitter.h>


namespace particles{

	//-----------------------------------------------------------------------------------
	processor::processor(base_emitter* em )   // конструктор
	: m_visible(true)
	, m_parent_emitter(em)
	{
		m_is_anim_texture_used = false; 		
		m_is_anim_texture_cycled = false;		
		m_texture_fps		= 25;		
		m_rnd_seed			= 0;		
		m_is_sparks			= false;		
		m_dt				= 0;
		m_rate_accum		= 0;
		m_is_fading			= false;
		first_time_init();
		m_parent_emitter	= 0;

		m_ngkx = 1.8f;

		m_rnd_frame			 = 0;
		m_is_play_tex_anim = true;

		m_color_alpha.add_key (0, math::Color (255, 255, 255, 255));
		m_color_alpha.add_key (1, math::Color (0, 0, 0, 0));
		m_life.add_key(1, 1.0f);
		m_size.add_key(1, 1.0f);
		m_rate.add_key(1, 10.0f);
		m_vel_spread_amp.add_key(1, 1.0f);
	}

	//-----------------------------------------------------------------------------------
	processor::~processor()
	{
	}

	//-----------------------------------------------------------------------------------
	void processor::update_particle(particle& p)
	{
		// отображение времени частица на интервал от 0 до 1 
		float t = p.time / p.ttl; 
		// time passed from last update
		float dt = p.time - p.old_time;

		if (t >= 1)
		{
			p.dead = true;
			p.color.a = 0;
			return;
		}

		p.color = m_color_alpha(t);

		math::vec3f velocity = m_velocity(t);
		
		// set_global acceleration
		const math::vec3f force = m_acting_force(m_normalized_time);

		p.vel += dt * (force * 0.5f / p.mass);
		
		// initial velocity
		velocity += p.vel;

		float resistance = math::length(m_scaling) * math::length(m_scaling) * (m_resistance(t) + 1.0f);
		float vel_spread_amp = m_vel_spread_amp(t);

		velocity = math::length(m_scaling) * math::length(m_scaling) * (p.vel_spread * vel_spread_amp + velocity ) / resistance;
			
		p.sum_vel = velocity * dt;

		p.pos += p.sum_vel;
		        
		p.size = math::length(m_scaling)*math::length(m_scaling) * m_size(t);

		p.rot_speed = m_spin(t);
		p.rotation += p.rot_speed + p.initial_spin;

		// save current update time
		p.old_time = p.time;
	}

	//-----------------------------------------------------------------------------------
	void processor::reset()								// ReStart процессор - сбросить его время на 0
	{
		m_is_fading = false;

		unsigned size = static_cast<unsigned>(m_particles.size());
		particle* pp = &(m_particles[0]);//[i]

		for (unsigned i = 0; i < size; ++i)
		{
			pp[i].dead = true;
			pp[i].size = 0.0f;
		}
	}

	//-----------------------------------------------------------------------------------
	void processor::first_time_init()
	{
		m_is_anim_texture_used		= false;
		m_modifiers_loaded		= false;
		m_ptank_inited		= false;
		m_visible			= true;
		m_additive_blend				= false;
		
		m_max_particles			= 0;
		m_normalized_time		= 0;
	}

	//-----------------------------------------------------------------------------------
	void processor::render()
	{
		if( !m_visible )
			return;

		update_particle();

		//if (!m_is_geometric)
		{
			m_tank.render(m_texture, m_parent_emitter);
		}
		//else
		//	geomRender();
	}

	//-----------------------------------------------------------------------------------
	void processor::update(float dt) 
	{
		m_dt += dt;

		m_normalized_time = m_parent_emitter->getTime();
		m_scaling = m_parent_emitter->scale();

		int m_acting_particles = 0;
		// здесь происходит апдейт партиклов
		//for (particles_iter it = m_particles.begin(); it != m_particles.end(); ++it)
		//	if (!(it->dead) && m_is_global)
		//	{
		//		it->pos += m_ngkx * m_parent_emitter->m_vCurDisplacement * (1.0f - (it->time / it->ttl));
		//	}

		if (m_dt > 0.01f)
		{
			// здесь происходит апдейт партиклов
			for (particles_iter it = m_particles.begin(); it != m_particles.end(); ++it)
				if (!it->dead)
				{
					++m_acting_particles;
					it->time += m_dt;
					update_particle(*it);
				}

			m_dt = 0;

			if (!m_is_fading)
				add_new_particles(m_max_particles - m_acting_particles);
		}
	}

	//-----------------------------------------------------------------------------------
	void processor::add_new_particles(int num2add)
	{
		// нет места для новых партиклов
		if(!num2add) 
			return;

		// скорость появления партиклов кол=во\за секунду
		float rate = m_rate.get_value(m_normalized_time) / 25.0f;
		m_rate_accum += rate;

		int to_add  = (int)m_rate_accum;

		if (to_add > num2add) 
			to_add = num2add;
		
		int added_num = 0;

		for(int i = 0; i < m_max_particles; ++i)
			if (m_particles[i].dead && to_add > 0){
				init_particle(m_particles[i]);
				to_add--;
				added_num++;
			}
			else if(to_add < 1)
				break;
		m_rate_accum -= added_num;
	}

	//-----------------------------------------------------------------------------------
	void processor::init_particle(particle& p)
	{
		m_parent_emitter->get_particle(p);
		p.dead = false;
		
		p.ttl = (m_life.get_value(m_normalized_time)
			+ rnd()* m_life_spread.get_value(m_normalized_time));// * 10.0f; // debug

		if (!p.ttl)
			p.dead = true;

		math::vec3f ivs = m_initial_vel_spread.get_value(m_normalized_time);

		p.vel_spread = math::vec3f(ivs[0] * (rnd()*2.0f - 1.0f),
							ivs[1] * (rnd()*2.0f - 1.0f),
							ivs[2] * (rnd()*2.0f - 1.0f));

		p.initial_spin = (rnd() * m_spin_spread.get_value(m_normalized_time))
						/(25.0f);
		
		//if (m_is_global){
		//	math::xform( p.initial_pos, local_trasform(), p.initial_pos );
		//	p.initial_vel = m_parent_emitter->getSpeed();// / 2.0f;
		//}

		p.pos = p.initial_pos;
		
		p.old_time = 0.0f;

		static math::unit_rand_2k lrnd;

		if (m_is_play_tex_anim)
		{
			float f = (float)m_rnd_frame;
			p.cur_tex_frame = lrnd() * f;

			if (p.cur_tex_frame  > m_ucTexFrames)
				p.cur_tex_frame = (float)m_ucTexFrames;
		}
		else
		{
			p.cur_tex_frame = lrnd() * (float)m_ucTexFrames;
		}

		update_particle(p);
	}

	//-----------------------------------------------------------------------------------
	void processor::debug_draw()
	{
		if( !m_visible )
			return;

		math::matrix44f m = local_trasform();

		//if (m_is_global)
		//	m = math::setTrans( m, math::vec3f(0,0,0) );

		math::vec3f center, vel;
		render::lines3d& line_manager = render::render_device::get().get_lines3d(); 
		for (particles_iter it = m_particles.begin(); it != m_particles.end(); ++it)
		{
			if ((*it).dead)
				continue;

			//if (!m_is_global)
			{
				center = m * (math::point3f)((*it).pos);
				vel = it->pos + m * (math::point3f)((*it).sum_vel*5.0f);
			}
			//else
			//{
			//	center = (*it).pos;
			//	vel = it->pos + (*it).sum_vel*5.0f;
			//}

			line_manager.add_quad( center, math::vec2f (it->size, it->size), 0 );	
			line_manager.add_line( center, vel, math::Green );
		}
	}
	//-----------------------------------------------------------------------------------
	void processor::update_particle()
	{
		//if (m_is_geometric)
		//	return;

		const math::matrix44f& ltm = local_trasform();

		m_tank.particles().resize( m_particles.size() );
		renderer::particle_t* array = &m_tank.particles().front();

		int i = 0;

		for (particles_iter it = m_particles.begin(); it != m_particles.end(); ++it)
		{
			particle &p = *it;

			//if (m_is_global)
			//	//*poss = p.pos;
			//	array[i].pos = p.pos;
			//else{
			//	//*poss = ltm.transform(p.pos);
			//	math::xform( array[i].pos, ltm, p.pos );
			//}
			array->pos = p.pos;

			array->size = math::vec2f( p.size, p.size );
			array->spin = p.rotation * 3.1415926f/180.0f;
			array->color = p.color.color;
			array->tile = 0; // TODO: implement animated texture

			array++;
		}

		m_tank.update();
	}

	//-----------------------------------------------------------------------------------
	void processor::texture(render::texture_ptr texture)
	{
		m_texture = texture;
	}

	//-----------------------------------------------------------------------------------
	const math::matrix44f& processor::local_trasform()
	{
		return m_parent_emitter->world_trasform();
	}

	//-----------------------------------------------------------------------------------
	void processor::load()
	{
		reset();
		additive_blend(m_additive_blend);
	}

	//-----------------------------------------------------------------------------------
	void processor::assign_children()
	{
		m_modifiers_loaded = true;
	}

	//-----------------------------------------------------------------------------------
	void processor::to_stream(io::write_stream& wf) const
	{
		wf  << file_version
			<< m_additive_blend
			<< m_scaling
			//<< m_is_global
			<< m_max_particles
			<< (m_texture ? m_texture->get_filename() : std::string())
			<< m_is_sparks
			//<< m_DffName
			<< m_rnd_seed
			<< m_ngkx
			<< m_texture_fps
			<< (m_is_anim_texture_cycled)
			<< (m_rate)
			<< (m_resistance)
			<< (m_spin)
			<< (m_spin_spread)
			<< (m_life)
			<< (m_life_spread)
			<< (m_size)
			<< (m_color_alpha)
			<< (m_acting_force)
			<< (m_velocity)
			<< (m_initial_vel_spread)
			<< (m_vel_spread_amp)
			<< m_rnd_frame
			<< m_is_play_tex_anim;
	}

	void processor::from_stream(io::read_stream& rf)
	{
		unsigned version;
		rf  >> version;
		if( version != file_version )
			assert(false);

		std::string texture_file_name;

		rf	>> m_additive_blend
			>> m_scaling
			//>> m_is_global
			>> m_max_particles
			>> texture_file_name
			>> m_is_sparks
			//>> m_DffName
			>> m_rnd_seed
			>> m_ngkx
			>> m_texture_fps
			>> (m_is_anim_texture_cycled)
			>> (m_rate)
			>> (m_resistance)
			>> (m_spin)
			>> (m_spin_spread)
			>> (m_life)
			>> (m_life_spread)
			>> (m_size)
			>> (m_color_alpha)
			>> (m_acting_force)
			>> (m_velocity)
			>> (m_initial_vel_spread)
			>> (m_vel_spread_amp)
			>> m_rnd_frame
			>> m_is_play_tex_anim;

		particles_limit( m_max_particles );

		m_texture = render::texture::create(texture_file_name);
	}

}