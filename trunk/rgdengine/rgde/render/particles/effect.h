#pragma once

#include <rgde/io/io.h>
#include <rgde/game/dynamic_object.h>
#include <rgde/core/xml_class.h>
#include <rgde/render/manager.h>


namespace particles
{
	class base_emitter;
	typedef boost::intrusive_ptr<base_emitter> emitter_ptr;

	class  effect : public render::rendererable
				  ,	public game::dynamic_object
	{
		static const unsigned file_version = 1001;
	public:
		typedef std::list<emitter_ptr> emitters_list;
		typedef emitters_list::iterator	 emitters_iter;

		effect();
		virtual ~effect();
				
		void add(emitter_ptr);
		void remove(emitter_ptr);

		const emitters_list& emitters() const {return m_emitters;}
		emitters_list&		 emitters()		  {return m_emitters;}

		void reset();

		inline void fade()	{set_emitters_to_fade(true); m_is_fading = true;}
		inline bool is_fading()	const {return m_is_fading;}		
		void debug_draw();

	protected:
		virtual render::renderable_info& get_renderable_info();

		virtual void to_stream(io::write_stream& wf);
		virtual void from_stream(io::read_stream& rf);

		virtual void update(float);
		void render();

		void set_emitters_to_fade(bool b);

	protected:
		math::frame_ptr	m_transform;
		emitters_list	m_emitters;
		bool			m_is_fading;
		float			old_time;
	};
}