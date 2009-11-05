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
	typedef boost::shared_ptr<class flame_fx> PFlameRenderer;

	class flame_fx : public rendererable, game::dynamic_object
	{
	public:
		static PFlameRenderer create(const std::string& tex, const math::Color& color, 
			const math::vec3f& pos, const math::vec2f& size, uint fps,  const std::vector<math::frame_ptr>& vVector);

		static PFlameRenderer create(const std::string& file_name, const std::vector<math::frame_ptr>& vVector);
	
		float getFPS() const	 {return m_fFPS;}
		void  setFPS(float fFPS) {m_fFPS = fFPS;}

	private:
		flame_fx(const std::string& tex, const math::Color& color, const math::vec3f& pos, 
			const math::vec2f& size, uint fps, const std::vector<math::frame_ptr>& vVector);

		void readTNF(const std::string& file_name);
		void render();

		virtual void update(float dt);

	private:
		int   m_nColumns;
		int   m_nRows;
		int   m_nFrames;
		int   m_nTotalColumns;
		int   m_nTotalRows;
		float m_fFPS;//Animation speed (in frames_vector Per Second)
		float m_fFramesToAdd;

		particles::renderer_ptr m_pTank;
		texture_ptr          m_pParticleTexture;
	};
}