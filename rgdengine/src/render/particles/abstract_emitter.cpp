#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/abstract_emitter.h>
#include <rgde/render/particles/processor.h>


namespace particles{
	//-----------------------------------------------------------------------------------
	void base_emitter::get_particle(particle& p) 
	{
		p = particle();

		p.mass = m_PMass.get_value(m_normalized_time)
			+ (m_Rand() * 2.0f - 1.0f) * m_PMassSpread.get_value(m_normalized_time);

		p.rotation = m_Rand() * m_PRotationSpread.get_value(m_normalized_time);
	}
	//-----------------------------------------------------------------------------------
	void base_emitter::add(processor* pp)
	{
		if (0 == pp)
			throw std::exception("base_emitter::add(): zero pointer!");

		m_processors.push_back(pp);
		pp->set_emitter(this);
	}
	//-----------------------------------------------------------------------------------
	void base_emitter::reset()
	{
		//m_vCurDisplacement = math::vec3f();
		//m_vCurSpeed = math::vec3f();
		//m_vOldPos = math::vec3f();

		m_time = 0;
		m_bIsEnded = false;
		m_normalized_time = 0;
		m_visible = true;

		for(processors_iter pi = m_processors.begin(); pi != m_processors.end(); ++pi)
			(*pi)->reset();
	}
	//////////////////////////////////////////////////////////////////////////
	void base_emitter::fade(bool b)
	{
		for (processors_iter it = m_processors.begin(); it != m_processors.end(); ++it)
			(*it)->fade(b);
	}
	//////////////////////////////////////////////////////////////////////////
	void base_emitter::update(float dt)
	{
		//if (m_bIsJustCreated && dt > 0.02f)
		//	dt = 0.02f;

		m_time += dt;

		// если вышли за переделы диапазона - надо вернуться
		if (!(m_time < m_fCycleTime))
		{
			if (!m_looped){
				m_bIsEnded = true;
				m_visible = false;
				return;
			}
			else 
			{
				m_bIsEnded = false;
			}

			int i = (int)(m_time / m_fCycleTime);
			m_time = m_time - m_fCycleTime * i;
		}
		
		m_normalized_time = m_time / m_fCycleTime;
		{
			//math::matrix44f m = local_trasform();

			//if (!m_bIsJustCreated)
			//{
			//	math::vec3f new_pos;
			//	math::setTrans(new_pos, m);
			//	m_vCurDisplacement = new_pos - m_vOldPos;
			//	m_vCurSpeed = m_vCurDisplacement / (dt * 20.0f);
			//	m_vOldPos = new_pos;
			//}
		}
		
		math::matrix44f m = world_trasform();

		math::invert( m );
		m_vPAcceleration = m_PAcceleration.get_value(m_normalized_time);
		//m_vAccelerationPrecomputed = m.transformVector(m_vPAcceleration);
		math::xform( m_vAccelerationPrecomputed, m, m_vPAcceleration );
		m_vGlobalVel = m_GlobalVelocity.get_value(m_normalized_time);
		//m_vGlobalVelPrecomputed = m.transformVector(m_vGlobalVel);
		math::xform( m_vGlobalVelPrecomputed, m, m_vGlobalVel );
		//m_vCurSpeedTransformed = m.transformVector(m_vCurSpeed);
		math::xform( m_vCurSpeedTransformed, m, m_vCurSpeed );

		for (processors_iter it = m_processors.begin(); it != m_processors.end(); ++it)
			(*it)->update(dt);

		//m_bIsJustCreated = false;
	}

	void base_emitter::render()
	{
		if (m_visible)
		{
			// 1-ми рендерятся не аддитивные партиклы
			for (processors_iter it = m_processors.begin(); it != m_processors.end(); ++it)
				if (!((*it)->additive_blend()))(*it)->render();

			// 2-ми рендерятся аддитивные партиклы
			for (processors_iter it = m_processors.begin(); it != m_processors.end(); ++it)
				if ((*it)->additive_blend())(*it)->render();
		}
	}


	base_emitter::base_emitter(type_t type) 
		: m_type(type)
		, m_time(0)
		, m_start_delay(0)
		, m_fCycleTime(5)
		, m_looped(true)
		, m_visible(true)
		, m_normalized_time(0)
	{
		m_PMass.add_key(1, 1.0f);
	}

	base_emitter::~base_emitter()
	{
		for( processors_iter it = m_processors.begin(); it != m_processors.end(); it++ )
			delete(*it);
		m_processors.clear();
	}
	
	void base_emitter::remove(processor* p)
	{
		m_processors.remove(p);
	}

	void base_emitter::to_stream(io::write_stream& wf) const
	{
		math::frame::to_stream (wf);

		wf	/*<< (int)m_type*/
			<< m_fCycleTime
			<< m_looped
			<< m_visible
			<< m_start_delay
			<< m_name
			<< (m_PMass)
			<< (m_PMassSpread)
			<< (m_PAcceleration)
			<< (m_GlobalVelocity)
			<< (m_velocity)
			<< (m_PVelSpread)
			<< (m_PRotationSpread);

		// Сохраняем процессоры частиц
		wf << (unsigned)m_processors.size();
		for( processors_list::const_iterator it = m_processors.begin(); it != m_processors.end(); ++it)
			wf << *(*it);
	}

	void base_emitter::from_stream(io::read_stream& rf)
	{
		math::frame::from_stream (rf);

		//{
		//	type t;
		//	rf	>> (int)t;
		//	assert(m_type == t);
		//}

		rf	>> m_fCycleTime
			>> m_looped
			>> m_visible
			>> m_start_delay
			>> m_name
			>> (m_PMass)
			>> (m_PMassSpread)
			>> (m_PAcceleration)
			>> (m_GlobalVelocity)
			>> (m_velocity)
			>> (m_PVelSpread)
			>> (m_PRotationSpread);

		// loading processors
		unsigned int num_of_processors;
		rf >> num_of_processors;
		for( unsigned i = 0; i < num_of_processors; ++i )
		{
			processor* proc = new processor();
			rf >> (*proc);
			add(proc);
			proc->load();
		}
	}

	void base_emitter::debug_draw()
	{
		math::frame::debug_draw();

		for( processors_iter it = m_processors.begin(); it != m_processors.end(); ++it )
			(*it)->debug_draw();
	}
}