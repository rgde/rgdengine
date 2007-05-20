//////////////////////////////////////////////////////////////////////////
// @date 06.01.06
// project: RGDE 
// description: Particles XML script services
// @todo perform full export, import and update
//////////////////////////////////////////////////////////////////////////
#include "engine.h"

#include "script/scriptParticles.h"

namespace script
{
	//----------------------------------------------------------------------------------------
	//									CEffectAdaptor
	//----------------------------------------------------------------------------------------
	void CEffectAdaptor::reset(std::string ignored)
	{
		ignored;
		object_->reset();
	}

	void CEffectAdaptor::fade(std::string ignored)
	{
		ignored;
		object_->fade();
	}

	void CEffectAdaptor::addEmitter(std::string str)
	{
	}

	void CEffectAdaptor::deleteEmitter(std::string str)
	{
	}

	void CEffectAdaptor::parse(void)
	{
		using namespace particles;
		// получаем эффект
		IEffect* pEffect = object_;

		IEffect::tEmitters& emitlist = pEffect->getEmitters();

		for(IEffect::tEmittersIter it = emitlist.begin(); it != emitlist.end(); ++it)
		{
			IEmitter* pEmitter = (*it);

			std::string name = pEmitter->getName();

			PEmitterAdaptor child = new CEmitterAdaptor(pEmitter, name);
			addChild(child);
			child->parse();
		}
	}


	////----------------------------------------------------------------------------------------
	////									CEmitterAdaptor
	////----------------------------------------------------------------------------------------


	void CEmitterAdaptor::reset(std::string ignored)
	{
		ignored;
		object_->reset();
	}

	void CEmitterAdaptor::debugInfo(std::string ignored)
	{
		ignored;
		object_->debugDraw();
	}

	// входные данные:
	void CEmitterAdaptor::addProcessor(std::string str)
	{
	}

	void CEmitterAdaptor::parse(void)
	{
		using namespace particles;
		
		// получаем эмиттер		
		IEmitter* pEmitter = object_;

		if(pEmitter->getEmitterType() != IEmitter::Type::Maya)
		{
			IAbstractEmitter* pAEmitter = static_cast<IAbstractEmitter*>(pEmitter);

			IAbstractEmitter::tPProcessors& proclist = pAEmitter->getProcessors();

			for(IAbstractEmitter::tPProcessorsIter jt = proclist.begin(); jt != proclist.end(); ++jt)
			{
				IParticlesProcessor* pProcessor = (*jt);

				std::string name = pProcessor->getName();

				PProcessorAdaptor child = new CProcessorAdaptor(pProcessor, name);
				addChild(child);
			}
		}
	}

	void CEmitterAdaptor::deleteProcessor(std::string str)
	{
	}


	////----------------------------------------------------------------------------------------
	////									CProcessorAdaptor
	////----------------------------------------------------------------------------------------


	void CProcessorAdaptor::loadTexture(std::string str)
	{
		object_->setTextureName(str);
	}

	void CProcessorAdaptor::reset(std::string ignored)
	{
		ignored;
		object_->reset();
	}

	void CProcessorAdaptor::debugInfo(std::string ignored)
	{
		ignored;
		object_->debugDraw();
	}

} // end script