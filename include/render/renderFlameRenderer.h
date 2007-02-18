#pragma once

#include "game/DynamicObject.h"
#include "render/renderManager.h"

namespace particles
{
	class CPTank;
	typedef boost::shared_ptr<class CPTank> PPTank;
}

namespace render
{
	typedef boost::shared_ptr<class CFlameRenderer> PFlameRenderer;

	class CFlameRenderer : public IRendererable, game::IDynamicObject
	{
	public:
		static PFlameRenderer Create(const std::string& tex, const math::Color& color, 
			const math::Vec3f& pos, const math::Vec2f& size, uint fps,  const std::vector<math::PFrame>& vVector);

		static PFlameRenderer Create(const std::string& strFileName, const std::vector<math::PFrame>& vVector);
	
		float getFPS() const	 {return m_fFPS;}
		void  setFPS(float fFPS) {m_fFPS = fFPS;}

	private:
		CFlameRenderer(const std::string& tex, const math::Color& color, const math::Vec3f& pos, 
			const math::Vec2f& size, uint fps, const std::vector<math::PFrame>& vVector);

		void readTNF(const std::string& strFileName);
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
		PTexture          m_pParticleTexture;
	};
}