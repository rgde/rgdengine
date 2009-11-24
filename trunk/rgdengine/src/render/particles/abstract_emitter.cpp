#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/abstract_emitter.h>
#include <rgde/render/particles/processor.h>


namespace particles{
	//-----------------------------------------------------------------------------------
	void base_emitter::get_particle(particle& p) 
	{
		p = particle();

		p.mass = m_PMass.get_value(m_fTimeNormalaized)
			+ (m_Rand() * 2.0f - 1.0f) * m_PMassSpread.get_value(m_fTimeNormalaized);

		p.rotation = m_Rand() * m_PRotationSpread.get_value(m_fTimeNormalaized);
	}
	//-----------------------------------------------------------------------------------
	void base_emitter::addProcessor(processor* pp)
	{
		if (0 == pp)
			throw std::exception("base_emitter::addProcessor(): zero pointer!");

		m_lProcessors.push_back(pp);
		pp->set_emitter(this);
	}
	//-----------------------------------------------------------------------------------
	void base_emitter::reset()
	{
		//m_vCurDisplacement = math::vec3f();
		//m_vCurSpeed = math::vec3f();
		//m_vOldPos = math::vec3f();

		m_fCurrentTime = 0;
		m_bIsEnded = false;
		m_fTimeNormalaized = 0;
		m_is_visible = true;

		for(processors_iter pi = m_lProcessors.begin(); pi != m_lProcessors.end(); ++pi)
			(*pi)->reset();
	}
	//////////////////////////////////////////////////////////////////////////
	void base_emitter::set_fade(bool b)
	{
		for (processors_iter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
			(*it)->set_fade(b);
	}
	//////////////////////////////////////////////////////////////////////////
	void base_emitter::update(float dt)
	{
		//if (m_bIsJustCreated && dt > 0.02f)
		//	dt = 0.02f;

		m_fCurrentTime += dt;

		// если вышли за переделы диапазона - надо вернуться
		if (!(m_fCurrentTime < m_fCycleTime))
		{
			if (!m_bIsCycling){
				m_bIsEnded = true;
				m_is_visible = false;
				return;
			}
			else 
			{
				m_bIsEnded = false;
			}

			int i = (int)(m_fCurrentTime / m_fCycleTime);
			m_fCurrentTime = m_fCurrentTime - m_fCycleTime * i;
		}
		
		m_fTimeNormalaized = m_fCurrentTime / m_fCycleTime;
		{
			//math::matrix44f m = get_local_tm();

			//if (!m_bIsJustCreated)
			//{
			//	math::vec3f new_pos;
			//	math::setTrans(new_pos, m);
			//	m_vCurDisplacement = new_pos - m_vOldPos;
			//	m_vCurSpeed = m_vCurDisplacement / (dt * 20.0f);
			//	m_vOldPos = new_pos;
			//}
		}
		
		math::matrix44f m = m_transform->get_full_tm();

		math::invert( m );
		m_vPAcceleration = m_PAcceleration.get_value(m_fTimeNormalaized);
		//m_vAccelerationPrecomputed = m.transformVector(m_vPAcceleration);
		math::xform( m_vAccelerationPrecomputed, m, m_vPAcceleration );
		m_vGlobalVel = m_GlobalVelocity.get_value(m_fTimeNormalaized);
		//m_vGlobalVelPrecomputed = m.transformVector(m_vGlobalVel);
		math::xform( m_vGlobalVelPrecomputed, m, m_vGlobalVel );
		//m_vCurSpeedTransformed = m.transformVector(m_vCurSpeed);
		math::xform( m_vCurSpeedTransformed, m, m_vCurSpeed );

		for (processors_iter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
			(*it)->update(dt);

		//m_bIsJustCreated = false;
	}

	void base_emitter::render()
	{
		if (m_is_visible)
		{
			// 1-ми рендерятся не аддитивные партиклы
			for (processors_iter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
				if (!((*it)->getIntenseMode()))(*it)->render();

			// 2-ми рендерятся аддитивные партиклы
			for (processors_iter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
				if ((*it)->getIntenseMode())(*it)->render();
		}
	}


	base_emitter::base_emitter(emitter::Type eType) : emitter(eType)
	{
		//m_bIsJustCreated = true;
		m_fCycleTime = 5.0f;
		m_bIsCycling = true;
		m_time_shift = 0.0f;

		m_fCurrentTime = 0.0f;
		m_is_visible = true; 

		m_PMass.add_key(1, 1.0f);

		m_fTimeNormalaized = 0;
		//if (!ms_Rand.isInited())
		//{
		//	ms_Rand.init(0.0f, 1.0f);
		//}

		//addProperty(new property<float>(m_fCycleTime,			"CycleTime",	"float"));
		//addProperty(new property<bool>(m_bIsCycling,			"IsCycling",	"bool"));
		//addProperty(new property<bool>(m_is_visible,			"IsVisible",	"bool"));
		//addProperty(new property<float>(m_time_shift,			"fTimeShift",	"float"));
		//
		//addProperty(new property<math::interpolatorf>(m_PMass,		"PMass",	"interpolatorf"));
		//addProperty(new property<math::interpolatorf>(m_PMassSpread,	"PMassSpread",	"interpolatorf"));
		//addProperty(new property<math::interpolatorf>(m_PRotationSpread,	"PRotationSpread",	"interpolatorf"));
		//addProperty(new property<math::interpolatorf>(m_velocity,	"PVelocity",	"interpolatorf"));
		//addProperty(new property<math::interpolatorf>(m_PVelSpread,	"PVelSpread",	"interpolatorf"));
		//addProperty(new property<math::interpolator_v3f>(m_PAcceleration,	"PAcceleration",	"interpolator_v3f"));
		//addProperty(new property<math::interpolator_v3f>(m_GlobalVelocity,	"GlobalVelocity",	"interpolator_v3f"));
		
		// public properties:
		//REGISTER_PROPERTY(fCycleTime,		float)
		//REGISTER_PROPERTY(bIsCycling,		bool)
		//REGISTER_PROPERTY(bIsVisible,		bool)
		//REGISTER_PROPERTY(fTimeShift,		float)
		//
		//REGISTER_PROPERTY(PMass,			math::interpolatorf)
		//REGISTER_PROPERTY(PMassSpread,		math::interpolatorf)
		//REGISTER_PROPERTY(PRotationSpread,	math::interpolatorf)
		//REGISTER_PROPERTY(PVelocity,		math::interpolatorf)
		//REGISTER_PROPERTY(PVelSpread,		math::interpolatorf)
		//REGISTER_PROPERTY(PAcceleration,	math::interpolator_v3f)
		//REGISTER_PROPERTY(GlobalVelocity,	math::interpolator_v3f)
	}

	base_emitter::~base_emitter()
	{
		for( processors_iter it = m_lProcessors.begin(); it != m_lProcessors.end(); it++ )
			delete(*it);
		m_lProcessors.clear();
	}

	//
	//math::vec3f base_emitter::getGlobalVelocity(bool global)
	//{
	//	if (!global)
	//		return m_vGlobalVelPrecomputed;
	//	else
	//		return m_vGlobalVel;
	//}
	//
	//math::vec3f base_emitter::getAcceleration(bool global)
	//{
	//	if (!global)
	//		return m_vAccelerationPrecomputed;
	//	else
	//		return m_vPAcceleration;
	//}
	
	void base_emitter::deleteProcessor(processor* p)
	{
		m_lProcessors.remove(p);
	}

	void base_emitter::to_stream(io::write_stream& wf) const
	{
		emitter::to_stream (wf);

		wf	<< m_fCycleTime
			<< m_bIsCycling
			<< m_is_visible
			<< m_time_shift
			<< m_name
			<< (m_PMass)
			<< (m_PMassSpread)
			<< (m_PAcceleration)
			<< (m_GlobalVelocity)
			<< (m_velocity)
			<< (m_PVelSpread)
			<< (m_PRotationSpread);

		// Сохраняем процессоры частиц
		wf << (unsigned)m_lProcessors.size();
		for( processors_list::const_iterator it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
			wf << *(*it);
	}

	void base_emitter::from_stream(io::read_stream& rf)
	{
		emitter::from_stream (rf);

		rf	>> m_fCycleTime
			>> m_bIsCycling
			>> m_is_visible
			>> m_time_shift
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
			addProcessor(proc);
			proc->load();
		}
	}

	void base_emitter::debug_draw()
	{
		m_transform->debug_draw();

		for( processors_iter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it )
			(*it)->debug_draw();
	}
}