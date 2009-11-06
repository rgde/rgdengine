#pragma once

#include <rgde/game/dynamic_object.h>
#include <rgde/render/manager.h>

namespace particles
{
	class renderer;
	typedef boost::shared_ptr<class renderer> renderer_ptr;
}

namespace render
{
	typedef boost::shared_ptr<class flame_fx> flame_fx_ptr;

	class flame_fx : public rendererable, game::dynamic_object
	{
	public:
		typedef const std::vector<math::frame_ptr> frames_vector;

		static flame_fx_ptr create(const std::string& tex, const math::Color& color, 
								   const math::vec3f& pos, const math::vec2f& size, 
								   uint fps,  frames_vector& frames);

		static flame_fx_ptr create(const std::string& file_name, frames_vector& frames);
	
		float get_fps() const	 {return m_fps;}
		void  set_fps(float fps) {m_fps = fps;}

	private:
		flame_fx(const std::string& tex, const math::Color& color, const math::vec3f& pos, 
				const math::vec2f& size, uint fps, const frames_vector& frames);

		void read_tnf(const std::string& file_name);
		void render();

		virtual void update(float dt);

	private:
		int   m_nColumns;
		int   m_nRows;
		int   m_nFrames;
		int   m_nTotalColumns;
		int   m_nTotalRows;
		float m_fps;//Animation speed (in frames_vector Per Second)
		float m_fFramesToAdd;

		particles::renderer_ptr m_tank;
		texture_ptr				m_particle_texture;
	};
}