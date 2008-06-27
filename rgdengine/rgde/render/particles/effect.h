#pragma once

#include <rgde/io/io.h>
#include <rgde/game/dynamic_object.h>
#include <rgde/core/xml_class.h>
#include <rgde/render/manager.h>

namespace script
{
	class CEffectAdaptor;
}

namespace particles
{
	class Emitter;

	class  Effect : /*public io::ISerializedObject,*/ public render::IRendererable, 
					 public game::dynamic_object, public core::XmlClass
	{
	public:
		typedef std::list<Emitter*> tEmitters;
		typedef tEmitters::iterator	 tEmittersIter;

		Effect();
		virtual ~Effect();
		
		friend class script::CEffectAdaptor;
		inline const math::Frame& getTransform() { return m_Transform; }
		
		void addEmitter(Emitter* em);
		void deleteEmitter(Emitter* em);

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
		math::Frame	m_Transform;
		tEmitters		m_Emitters;						// Ёмиттеры
		bool			m_bIsFading;
		float			m_fOldTime;

	private:
		static const unsigned ms_nVersion = 1001;
	};
}