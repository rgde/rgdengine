#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/effect.h>

// Майя эмиттер
#include <rgde/render/particles/static_emitter.h>
#include <rgde/render/particles/tank.h>

// Абстрактные эмиттеры
#include <rgde/render/particles/box_emitter.h>
#include <rgde/render/particles/spherical_emitter.h>

namespace particles
{
	//-----------------------------------------------------------------------------------
	effect::effect()
	: render::rendererable(9)
	, old_time(0)
	, m_bIsFading(false)
	, core::meta_class("ParticleEffect")
	{	
		m_renderInfo.pRenderFunc		= boost::bind( &effect::render, this );
		m_renderInfo.pDebugRenderFunc	= boost::bind(&effect::debugDraw, this);
		
		// public properties:
		//REGISTER_PROPERTY(Transform, math::frame)
	}
	//-----------------------------------------------------------------------------------
	effect::~effect()
	{
		for (tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
			delete( *it );
		m_Emitters.clear();
	}
	//-----------------------------------------------------------------------------------
	void effect::reset()
	{
		m_bIsFading = false;
		setEmittersToFade(m_bIsFading);

		for (tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
			(*it)->reset();
	}
	//-----------------------------------------------------------------------------------
	void effect::render()
	{
		for (tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
			(*it)->render();
	}
	//-----------------------------------------------------------------------------------
	void effect::setEmittersToFade(bool b)
	{
		for(tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
			(*it)->setFade(b);
	}
	//-----------------------------------------------------------------------------------
	void effect::update(float fDeltaTime)
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
	void effect::add(emitter* em)
	{
		assert(0 != em);
		m_Emitters.push_back(em);
		m_Transform.add(&em->getTransform());
	}
	
	//-----------------------------------------------------------------------------------
	void effect::remove(emitter* em)
	{
		assert(0 != em);
		m_Transform.remove(&em->getTransform());		
		m_Emitters.remove(em);
	}

	//-----------------------------------------------------------------------------------
	void effect::debugDraw()
	{
		m_Transform.debugDraw();
		for( tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); ++it )
			(*it)->debugDraw();
	}
	//-----------------------------------------------------------------------------------
	void effect::toStream(io::write_stream& wf)
	{
		wf << file_version;
		wf << m_Transform;

		// Сохраняем абстрактные эмитеры
		wf << (unsigned)m_Emitters.size();
		for( tEmittersIter it = m_Emitters.begin(); it != m_Emitters.end(); it++ )
			wf << *(*it);
	}
	//----------------------------------------------------------------------------------
	render::SRenderableInfo& effect::getRenderableInfo()
	{
		return m_renderInfo;		
	}
	//-----------------------------------------------------------------------------------
	void effect::fromStream(io::read_stream& rf)
	{
		unsigned version;
		rf >> version;
		if (version != file_version)
			throw std::exception("pfx::effect::fromStream(...): Unknown version !");
		
		rf >> m_Transform;

		unsigned size;
		rf >> size;
		for(unsigned i = 0; i < size; i++)
		{
			unsigned em_type = 0;
			rf >> em_type;
			emitter::Type type = static_cast<emitter::Type> (em_type);
		
			emitter* em;
			switch(type)
			{
				case emitter::Static:		em = new static_emitter;	break;
				case emitter::Spherical:	em = new spherical_emitter;	break;
				case emitter::Box:			em = new box_emitter;		break;
			}
			rf >> (*em);
			add(em);
		}
	}
}