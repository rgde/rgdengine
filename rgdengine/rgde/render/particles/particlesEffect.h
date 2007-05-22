#pragma once

#include <rgde/io/io.h>
#include <rgde/game/DynamicObject.h>
#include <rgde/core/xmlClass.h>
#include <rgde/render/renderManager.h>

namespace script
{
	class CEffectAdaptor;
}

namespace particles
{
	class IEmitter;

	class  IEffect : public io::ISerializedObject, public render::IRendererable, 
					 public game::IDynamicObject, public core::XmlClass
	{
	public:
		typedef std::list<IEmitter*> tEmitters;
		typedef tEmitters::iterator	 tEmittersIter;

		IEffect();
		virtual ~IEffect();
		
		friend class script::CEffectAdaptor;
		inline const math::CFrame& getTransform() { return m_Transform; }
		
		void addEmitter(IEmitter* em);
		void deleteEmitter(IEmitter* em);

		tEmitters& getEmitters() {return m_Emitters;}

		void reset();

		inline void fade(){setEmittersToFade(true); m_bIsFading = true;}
		inline bool isFading(){return m_bIsFading;}

		virtual render::SRenderableInfo&	getRenderableInfo();

	protected:
		virtual void toStream(io::IWriteStream& wf);
		virtual void fromStream(io::IReadStream& rf);

		virtual void update(float);

		void render();
		void debugDraw();

		void setEmittersToFade(bool b);

	protected:
		math::CFrame	m_Transform;
		tEmitters		m_Emitters;						// Ёмиттеры
		bool			m_bIsFading;
		float			m_fOldTime;

	private:
		static const unsigned ms_nVersion = 1001;
	};
}