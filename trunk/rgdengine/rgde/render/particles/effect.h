#pragma once

#include <rgde/io/io.h>
#include <rgde/game/dynamic_object.h>
#include <rgde/core/xml_class.h>
#include <rgde/render/manager.h>


namespace particles
{
	class emitter;

	class  effect : /*public io::serialized_object,*/ public render::rendererable, 
					 public game::dynamic_object, public core::meta_class
	{
	public:
		typedef std::list<emitter*> tEmitters;
		typedef tEmitters::iterator	 tEmittersIter;

		effect();
		virtual ~effect();
		
		inline const math::Frame& getTransform() { return m_Transform; }
		
		void add(emitter* em);
		void remove(emitter* em);

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
		tEmitters		m_Emitters;						// emitters
		bool			m_bIsFading;
		float			old_time;

	private:
		static const unsigned file_version = 1001;
	};
}