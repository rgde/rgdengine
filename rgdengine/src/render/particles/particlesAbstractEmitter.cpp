#include "precompiled.h"

#include "render/particles/particlesMain.h"
#include "render/particles/particlesAbstractEmitter.h"
#include "render/particles/particlesParticlesProcessor.h"


namespace particles{
	//-----------------------------------------------------------------------------------
	void IAbstractEmitter::getParticle(Particle& p) 
	{
		p = Particle();

		p.mass = m_PMass.getValue(m_fTimeNormalaized)
			+ (m_Rand() * 2.0f - 1.0f) * m_PMassSpread.getValue(m_fTimeNormalaized);

		p.rotation = m_Rand() * m_PRotationSpread.getValue(m_fTimeNormalaized);
	}
	//-----------------------------------------------------------------------------------
	void IAbstractEmitter::addProcessor(IParticlesProcessor* pp)
	{
		if (0 == pp)
			throw std::exception("AbstractEmitter::addProcessor(): zero pointer!");

		m_lProcessors.push_back(pp);
		pp->setEmitter(this);
	}
	//-----------------------------------------------------------------------------------
	void IAbstractEmitter::reset()
	{
		//m_vCurDisplacement = math::Vec3f();
		//m_vCurSpeed = math::Vec3f();
		//m_vOldPos = math::Vec3f();

		m_fCurrentTime = 0;
		m_bIsEnded = false;
		m_fTimeNormalaized = 0;
		m_bIsVisible = true;

		for(tPProcessorsIter pi = m_lProcessors.begin(); pi != m_lProcessors.end(); ++pi)
			(*pi)->reset();
	}
	//////////////////////////////////////////////////////////////////////////
	void IAbstractEmitter::setFade(bool b)
	{
		for (tPProcessorsIter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
			(*it)->setFade(b);
	}
	//////////////////////////////////////////////////////////////////////////
	void IAbstractEmitter::update(float dt)
	{
		//if (m_bIsJustCreated && dt > 0.02f)
		//	dt = 0.02f;

		m_fCurrentTime += dt;

		// если вышли за переделы диапазона - надо вернуться
		if (!(m_fCurrentTime < m_fCycleTime))
		{
			if (!m_bIsCycling){
				m_bIsEnded = true;
				m_bIsVisible = false;
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

		for (tPProcessorsIter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
			(*it)->update(dt);

		//m_bIsJustCreated = false;
	}

	void IAbstractEmitter::render()
	{
		if (m_bIsVisible)
		{
			// 1-ми рендерятся не аддитивные партиклы
			for (tPProcessorsIter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
				if (!((*it)->getIntenseMode()))(*it)->render();

			// 2-ми рендерятся аддитивные партиклы
			for (tPProcessorsIter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
				if ((*it)->getIntenseMode())(*it)->render();
		}
	}


	IAbstractEmitter::IAbstractEmitter(IEmitter::Type eType) : IEmitter(eType)
	{
		//m_bIsJustCreated = true;
		m_fCycleTime = 5.0f;
		m_bIsCycling = true;
		m_fTimeShift = 0.0f;

		m_fCurrentTime = 0.0f;
		m_bIsVisible = true; 

		m_PMass.addKey(1, 1.0f);

		m_fTimeNormalaized = 0;
		//if (!ms_Rand.isInited())
		//{
		//	ms_Rand.init(0.0f, 1.0f);
		//}

		//addProperty(new TProperty<float>(m_fCycleTime,			"CycleTime",	"float"));
		//addProperty(new TProperty<bool>(m_bIsCycling,			"IsCycling",	"bool"));
		//addProperty(new TProperty<bool>(m_bIsVisible,			"IsVisible",	"bool"));
		//addProperty(new TProperty<float>(m_fTimeShift,			"fTimeShift",	"float"));
		//
		//addProperty(new TProperty<math::FloatInterp>(m_PMass,		"PMass",	"FloatInterp"));
		//addProperty(new TProperty<math::FloatInterp>(m_PMassSpread,	"PMassSpread",	"FloatInterp"));
		//addProperty(new TProperty<math::FloatInterp>(m_PRotationSpread,	"PRotationSpread",	"FloatInterp"));
		//addProperty(new TProperty<math::FloatInterp>(m_PVelocity,	"PVelocity",	"FloatInterp"));
		//addProperty(new TProperty<math::FloatInterp>(m_PVelSpread,	"PVelSpread",	"FloatInterp"));
		//addProperty(new TProperty<math::Vec3Interp>(m_PAcceleration,	"PAcceleration",	"Vec3Interp"));
		//addProperty(new TProperty<math::Vec3Interp>(m_GlobalVelocity,	"GlobalVelocity",	"Vec3Interp"));
		
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

	IAbstractEmitter::~IAbstractEmitter()
	{
		for( tPProcessorsIter it = m_lProcessors.begin(); it != m_lProcessors.end(); it++ )
			delete(*it);
		m_lProcessors.clear();
	}

	//
	//math::Vec3f IAbstractEmitter::getGlobalVelocity(bool global)
	//{
	//	if (!global)
	//		return m_vGlobalVelPrecomputed;
	//	else
	//		return m_vGlobalVel;
	//}
	//
	//math::Vec3f IAbstractEmitter::getAcceleration(bool global)
	//{
	//	if (!global)
	//		return m_vAccelerationPrecomputed;
	//	else
	//		return m_vPAcceleration;
	//}
	
	void IAbstractEmitter::deleteProcessor(IParticlesProcessor* p)
	{
		m_lProcessors.remove(p);
	}

	void IAbstractEmitter::toStream(io::IWriteStream& wf) const
	{
		IEmitter::toStream (wf);

		wf	<< m_fCycleTime
			<< m_bIsCycling
			<< m_bIsVisible
			<< m_fTimeShift
			<< m_strName
			<< (m_PMass)
			<< (m_PMassSpread)
			<< (m_PAcceleration)
			<< (m_GlobalVelocity)
			<< (m_PVelocity)
			<< (m_PVelSpread)
			<< (m_PRotationSpread);

		// Сохраняем процессоры частиц
		wf << (unsigned)m_lProcessors.size();
		for( tPProcessors::const_iterator it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it)
			wf << *(*it);
	}

	void IAbstractEmitter::fromStream(io::IReadStream& rf)
	{
		IEmitter::fromStream (rf);

		rf	>> m_fCycleTime
			>> m_bIsCycling
			>> m_bIsVisible
			>> m_fTimeShift
			>> m_strName
			>> (m_PMass)
			>> (m_PMassSpread)
			>> (m_PAcceleration)
			>> (m_GlobalVelocity)
			>> (m_PVelocity)
			>> (m_PVelSpread)
			>> (m_PRotationSpread);

		// Сохраняем процессоры частиц
		unsigned int nProcessors;
		rf >> nProcessors;
		for( unsigned i = 0; i < nProcessors; i++ )
		{
			IParticlesProcessor* processor = new IParticlesProcessor();
			rf >> (*processor);
			addProcessor(processor);
			processor->load();
		}
	}

	void IAbstractEmitter::debugDraw()
	{
		m_Transform.debugDraw();

		for( tPProcessorsIter it = m_lProcessors.begin(); it != m_lProcessors.end(); ++it )
			(*it)->debugDraw();
	}
}