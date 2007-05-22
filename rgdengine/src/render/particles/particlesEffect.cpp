#include "precompiled.h"

#include <rgde/render/particles/particlesMain.h>
#include <rgde/render/particles/particlesEffect.h>

// Майя эмиттер
#include <rgde/render/particles/particlesMayaEmitter.h>
#include <rgde/render/particles/particlesParticleTank.h>

// Абстрактные эмиттеры
#include <rgde/render/particles/particlesBoxEmitter.h>
#include <rgde/render/particles/particlesSphericalEmitter.h>


namespace particles
{
	//-----------------------------------------------------------------------------------
	IEffect::IEffect() : render::IRendererable(9), m_fOldTime(0), m_bIsFading(false), 
						 core::XmlClass("ParticleEffect")
	{	
		m_renderInfo.pRenderFunc		= boost::bind( &IEffect::render, this );
		m_renderInfo.pDebugRenderFunc	= boost::bind(&IEffect::debugDraw, this);
		
		// public properties:
		//REGISTER_PROPERTY(Transform, math::CFrame)
	}
	//-----------------------------------------------------------------------------------
	IEffect::~IEffect()
	{
		for (tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
			delete( *it );
		m_Emitters.clear();
	}
	//-----------------------------------------------------------------------------------
	void IEffect::reset()
	{
		m_bIsFading = false;
		setEmittersToFade(m_bIsFading);

		for (tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
			(*it)->reset();
	}
	//-----------------------------------------------------------------------------------
	void IEffect::render()
	{
		for (tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
			(*it)->render();
	}
	//-----------------------------------------------------------------------------------
	void IEffect::setEmittersToFade(bool b)
	{
		for(tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
			(*it)->setFade(b);
	}
	//-----------------------------------------------------------------------------------
	void IEffect::update(float fDeltaTime)
	{
		static float dt = 0.02f;

		{ // проводим апдейт только 25 раз в секунду
			dt += fDeltaTime;
			if (dt < 0.02f) return;
		}
		
		for (tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
			(*it)->update(dt);

		dt = 0;
	}

	//-----------------------------------------------------------------------------------
	void IEffect::addEmitter(IEmitter* em)
	{
		assert(0 != em);
		m_Emitters.push_back(em);
		m_Transform.addChild(&em->getTransform());
	}
	
	//-----------------------------------------------------------------------------------
	void IEffect::deleteEmitter(IEmitter* em)
	{
		assert(0 != em);
		m_Transform.removeChild(&em->getTransform());		
		m_Emitters.remove(em);
	}

	//-----------------------------------------------------------------------------------
	void IEffect::debugDraw()
	{
		m_Transform.debugDraw();
		for( tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it )
			(*it)->debugDraw();
	}
	//-----------------------------------------------------------------------------------
	void IEffect::toStream(io::IWriteStream& wf)
	{
		wf << ms_nVersion;
		wf << m_Transform;

		// Сохраняем абстрактные эмитеры
		wf << (unsigned)m_Emitters.size();
		for( tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); it++ )
			wf << *(*it);
	}
	//----------------------------------------------------------------------------------
	render::SRenderableInfo&	IEffect::getRenderableInfo()
	{
		return m_renderInfo;		
	}
	//-----------------------------------------------------------------------------------
	void IEffect::fromStream(io::IReadStream& rf)
	{
		unsigned version;
		rf >> version;
		if (version != ms_nVersion)
			throw std::exception("pfx::IEffect::fromStream(...): Unknown version !");
		
		rf >> m_Transform;

		unsigned size;
		rf >> size;
		for(unsigned i = 0; i < size; i++)
		{
			unsigned em_type = 0;
			rf >> em_type;
			IEmitter::Type type = static_cast<IEmitter::Type> (em_type);
		
			IEmitter* em;
			switch(type)
			{
				case IEmitter::Maya:		em = new IMayaEmitter;		break;
				case IEmitter::Spherical:	em = new ISphericalEmitter;	break;
				case IEmitter::Box:			em = new IBoxEmitter;		break;
			}
			rf >> (*em);
			addEmitter(em);
		}
	}
}