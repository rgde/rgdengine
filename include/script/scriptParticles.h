//////////////////////////////////////////////////////////////////////////
// @author Guardian,  
// @date 30.12.05
// project: RGDE 
// description: Particles Exporter
// @todo perform full export, import and update
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "../render/particles/particlesEffect.h"
#include "../render/particles/particlesEmitter.h"
#include "../render/particles/particlesAbstractEmitter.h"
#include "../render/particles/particlesParticlesProcessor.h"
#include "../render/particles/particlesMayaEmitter.h"
#include "../render/particles/particlesBoxEmitter.h"
#include "../render/particles/particlesSphericalEmitter.h"


namespace script
{

	// Template for adaptor class:
	//class CXXXAdaptor : public TAbstractAdaptor<XXX>
	//{
	//public:
	//	CXXXAdaptor(XXX* p, std::string& name) : TAbstractAdaptor<XXX>(p, name)
	//	{
	//		// register generic functions here
	//		REGISTER_FUNCTION(func, CXXXAdaptor, &func)
	//	}
	//	~CXXXAdaptor(){};

	//	// this method can be implemented here, if target object XXX have any children
	//	void parse(void);

	//	// declare generic functions here
	//	void func(std::string);

	//};


	class CProcessorAdaptor;
	class CEmitterAdaptor;

	//----------------------------------------------------------------------------------------
	//									CEffectAdaptor
	//----------------------------------------------------------------------------------------
	// Этот адаптор позволяет вызывать методы IEffect как обощенные функции
	class CEffectAdaptor : public TAbstractAdaptor<particles::IEffect>
	{
	public:

		CEffectAdaptor(particles::IEffect* p, std::string& name) : TAbstractAdaptor<particles::IEffect>(p, name)
		{
			//здесь регистрируем все доступные клиенту методы и свойства:
			REGISTER_FUNCTION(addEmitter,		CEffectAdaptor, &script::CEffectAdaptor::addEmitter)
			REGISTER_FUNCTION(deleteEmitter,	CEffectAdaptor, &script::CEffectAdaptor::deleteEmitter)
			REGISTER_FUNCTION(reset,			CEffectAdaptor, &script::CEffectAdaptor::reset)
			REGISTER_FUNCTION(fade,				CEffectAdaptor, &script::CEffectAdaptor::fade)
		}
		~CEffectAdaptor(){};

		void addEmitter(std::string);
		void deleteEmitter(std::string);
		void reset(std::string);
		void fade(std::string);

		void parse(void);

	};
	typedef boost::intrusive_ptr<CEffectAdaptor> PEffectAdaptor;


	//----------------------------------------------------------------------------------------
	//									CEmitterAdaptor
	//----------------------------------------------------------------------------------------
	
	class CEmitterAdaptor : public TAbstractAdaptor<particles::IEmitter>
	{
	public:

		CEmitterAdaptor(particles::IEmitter* p, std::string& name) : TAbstractAdaptor<particles::IEmitter>(p, name)
		{
			//здесь регистрируем все доступные клиенту методы и свойства
			REGISTER_FUNCTION(reset,			CEmitterAdaptor, &script::CEmitterAdaptor::reset)
			REGISTER_FUNCTION(debugInfo,		CEmitterAdaptor, &script::CEmitterAdaptor::debugInfo)
			
			if(p->getEmitterType() != particles::IEmitter::Type::Maya)
			{
				REGISTER_FUNCTION(addProcessor,		CEmitterAdaptor, &script::CEmitterAdaptor::addProcessor)
				REGISTER_FUNCTION(deleteProcessor,	CEmitterAdaptor, &script::CEmitterAdaptor::deleteProcessor)
			}
		}

		void reset(std::string);
		void debugInfo(std::string);
		void addProcessor(std::string);	
		void deleteProcessor(std::string);

		void parse(void);

	};
	typedef boost::intrusive_ptr<CEmitterAdaptor> PEmitterAdaptor;
	
	//----------------------------------------------------------------------------------------
	//									CProcessorAdaptor
	//----------------------------------------------------------------------------------------
	class CProcessorAdaptor : public TAbstractAdaptor<particles::IParticlesProcessor>
	{
	public:

		CProcessorAdaptor(particles::IParticlesProcessor* p, std::string& name) : TAbstractAdaptor<particles::IParticlesProcessor>(p, name)
		{
			//здесь регистрируем все доступные клиенту методы и свойства
			REGISTER_FUNCTION(reset,		CProcessorAdaptor, &script::CProcessorAdaptor::reset)
			REGISTER_FUNCTION(debugInfo,	CProcessorAdaptor, &script::CProcessorAdaptor::debugInfo)
			REGISTER_FUNCTION(loadTexture,	CProcessorAdaptor, &script::CProcessorAdaptor::loadTexture)
		}

	
		void loadTexture(std::string);
		void reset(std::string);
		void debugInfo(std::string);
	};

	typedef boost::intrusive_ptr<CProcessorAdaptor> PProcessorAdaptor;
}// end of script