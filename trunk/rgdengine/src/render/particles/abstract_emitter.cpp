#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/abstract_emitter.h>
#include <rgde/render/particles/processor.h>


namespace particles{
	//-----------------------------------------------------------------------------------
	void base_emitter::get_particle(particle& p) 
	{
		p = particle();

		p.mass = m_PMass.getValue(m_fTimeNormalaized)
			+ (m_Rand() * 2.0f - 1.0f) * m_PMassSpread.getValue(m_fTimeNormalaized);

		p.rotation = m_Rand() * m_PRotationSpread.getValue(m_fTimeNormalaized);
	}
	//-----------------------------------------------------------------------------------
	void base_emitter::addProcessor(processor* pp)
	{
		if (0 == pp)
			throw std::exception("base_emitter::addProcessor(): zero pointer!");

		m_lProcessors.push_back(pp);
		pp->setEmitter(this);
	}
	//-----------------------------------------------------------------------------------
	void base_emitter::reset()
	{
		//m_vCurDisplacement = math::Vec3f();
		//m_vCurSpeed = math::Vec3f();
		//m_vOldPos = math::Vec3f();

		m_fCurrentTime = 0;
		m_bIsEnded = false;
		m_fTimeNormalaized = 0;
		m_is_visible = true;

		for(processors_iter pi = m_lProcessors.begin(); pi != m_lProcessors.end(); ++pi)
			(*pi)->reset();
	}
	//////////////////////////////////////////////////////////////////////////
	void base_emitter::setFade(bool b)
	{
		for (processors_iter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
			(*it)->setFade(b);
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
			//math::Matrix44f m = getLTM();

			//if (!m_bIsJustCreated)
			//{
			//	math::Vec3f new_pos;
			//	math::setTrans(new_pos, m);
			//	m_vCurDisplacement = new_pos - m_vOldPos;
			//	m_vCurSpeed = m_vCurDisplacement / (dt * 20.0f);
			//	m_vOldPos = new_pos;
			//}
		}
		
		math::Matrix44f m = m_Transform.getFullTransform();

		math::invert( m );
		m_vPAcceleration = m_PAcceleration.getValue(m_fTimeNormalaized);
		//m_vAccelerationPrecomputed = m.transformVector(m_vPAcceleration);
		math::xform( m_vAccelerationPrecomputed, m, m_vPAcceleration );
		m_vGlobalVel = m_GlobalVelocity.getValue(m_fTimeNormalaized);
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
		//addProperty(new property<math::FloatInterp>(m_PMass,		"PMass",	"FloatInterp"));
		//addProperty(new property<math::FloatInterp>(m_PMassSpread,	"PMassSpread",	"FloatInterp"));
		//addProperty(new property<math::FloatInterp>(m_PRotationSpread,	"PRotationSpread",	"FloatInterp"));
		//addProperty(new property<math::FloatInterp>(m_PVelocity,	"PVelocity",	"FloatInterp"));
		//addProperty(new property<math::FloatInterp>(m_PVelSpread,	"PVelSpread",	"FloatInterp"));
		//addProperty(new property<math::Vec3Interp>(m_PAcceleration,	"PAcceleration",	"Vec3Interp"));
		//addProperty(new property<math::Vec3Interp>(m_GlobalVelocity,	"GlobalVelocity",	"Vec3Interp"));
		
		// public properties:
		//REGISTER_PROPERTY(fCycleTime,		float)
		//REGISTER_PROPERTY(bIsCycling,		bool)
		//REGISTER_PROPERTY(bIsVisible,		bool)
		//REGISTER_PROPERTY(fTimeShift,		float)
		//
		//REGISTER_PROPERTY(PMass,			math::FloatInterp)
		//REGISTER_PROPERTY(PMassSpread,		math::FloatInterp)
		//REGISTER_PROPERTY(PRotationSpread,	math::FloatInterp)
		//REGISTER_PROPERTY(PVelocity,		math::FloatInterp)
		//REGISTER_PROPERTY(PVelSpread,		math::FloatInterp)
		//REGISTER_PROPERTY(PAcceleration,	math::Vec3Interp)
		//REGISTER_PROPERTY(GlobalVelocity,	math::Vec3Interp)
	}

	base_emitter::~base_emitter()
	{
		for( processors_iter it = m_lProcessors.begin(); it != m_lProcessors.end(); it++ )
			delete(*it);
		m_lProcessors.clear();
	}

	//
	//math::Vec3f base_emitter::getGlobalVelocity(bool global)
	//{
	//	if (!global)
	//		return m_vGlobalVelPrecomputed;
	//	else
	//		return m_vGlobalVel;
	//}
	//
	//math::Vec3f base_emitter::getAcceleration(bool global)
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

	void base_emitter::toStream(io::write_stream& wf) const
	{
		emitter::toStream (wf);

		wf	<< m_fCycleTime
			<< m_bIsCycling
			<< m_is_visible
			<< m_time_shift
			<< m_name
			<< (m_PMass)
			<< (m_PMassSpread)
			<< (m_PAcceleration)
			<< (m_GlobalVelocity)
			<< (m_PVelocity)
			<< (m_PVelSpread)
			<< (m_PRotationSpread);

		// Сохраняем процессоры частиц
		wf << (unsigned)m_lProcessors.size();
		for( processors_list::const_iterator it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
			wf << *(*it);
	}

	void base_emitter::fromStream(io::read_stream& rf)
	{
		emitter::fromStream (rf);

		rf	>> m_fCycleTime
			>> m_bIsCycling
			>> m_is_visible
			>> m_time_shift
			>> m_name
			>> (m_PMass)
			>> (m_PMassSpread)
			>> (m_PAcceleration)
			>> (m_GlobalVelocity)
			>> (m_PVelocity)
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
		m_Transform.debug_draw();

		for( processors_iter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it )
			(*it)->debug_draw();
	}
}