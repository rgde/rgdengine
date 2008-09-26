#pragma once

#include <rgde/game/dynamic_object.h>
#include <rgde/render/manager.h>

namespace particles
{
	class PTank;
	typedef boost::shared_ptr<class PTank> PPTank;
}

namespace render
{
	typedef boost::shared_ptr<class FlameRenderer> PFlameRenderer;

	class FlameRenderer : public rendererable, game::dynamic_object
	{
	public:
		static PFlameRenderer create(const std::string& tex, const math::Color& color, 
			const math::Vec3f& pos, const math::Vec2f& size, uint fps,  const std::vector<math::frame_ptr>& vVector);

		static PFlameRenderer create(const std::string& file_name, const std::vector<math::frame_ptr>& vVector);
	
		float getFPS() const	 {return m_fFPS;}
		void  setFPS(float fFPS) {m_fFPS = fFPS;}

	private:
		FlameRenderer(const std::string& tex, const math::Color& color, const math::Vec3f& pos, 
			const math::Vec2f& size, uint fps, const std::vector<math::frame_ptr>& vVector);

		void readTNF(const std::string& file_name);
		void render();

		virtual void update(float dt);

	private:
		int   m_nColumns;
		int   m_nRows;
		int   m_nFrames;
		int   m_nTotalColumns;
		int   m_nTotalRows;
		float m_fFPS;//Animation speed (in Frames Per Second)
		float m_fFramesToAdd;

		particles::PPTank m_pTank;
		texture_ptr          m_pParticleTexture;
	};
}