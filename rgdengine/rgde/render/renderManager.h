#pragma once

#include <rgde/math/mathTransform.h>

#include <rgde/render/renderTexture.h>
#include <rgde/render/renderFont.h>
#include <rgde/render/renderEffect.h>
#include <rgde/render/renderFog.h>
#include <rgde/render/renderMaterial.h>
#include <rgde/render/renderBinders.h>

namespace render
{
	enum FillMode 
	{
		Point               = 1,
		WireFrame           = 2,
		Solid               = 3,
		FillMode_OutOfRange = 0x7fffffff, /* force 32-bit size enum */
	};

	class CMesh;

	class CRenderManager
	{
		friend class IRendererable;

	protected:
		CRenderManager();
		virtual ~CRenderManager();

	public:
		void clear();
		void renderScene();
		
		void enableVolumes( bool flag )		{ m_bVolumes = flag; }
		bool isVolumeDrawing() const		{ return m_bVolumes; }

		void enableLighting(bool bLightingEnabled)  { m_bLightingEnabled = bLightingEnabled; }
		bool isLightingEnabled() const		{ return m_bLightingEnabled; }

		void setFillMode(FillMode mode)		{ m_nFillMode = mode; }
		FillMode  getFillMode() const			{ return m_nFillMode; }

		PEffect& getDefaultEffect();
		PFont&   getDefaultFont();
		const CFog&    getDefaultFog()	const {return m_pDefaultFog;}

		PTexture& getBlackTexture();
		PTexture& getWhiteTexture();
		PTexture& getDefaultNormalMap();

		void setCurrentFog(const CFog& fog);
		const CFog& getCurrentFog() const	{return m_pCurrentFog;}

	private:
		void add(IRendererable* r);
		void remove(IRendererable* r);

		void createBinder();

	protected:
		typedef std::vector<IRendererable*> Renderables;
		Renderables m_lRenderables;
		//std::list<IRendererable*>	           m_lRenderables;
		bool						           m_bVolumes;
		bool                                   m_bLightingEnabled;
		FillMode                               m_nFillMode;

		PEffect                                m_pDefaultEffect;
		PFont                                  m_pDefaultFont;
		CFog                                   m_pDefaultFog;

		PTexture                               m_pBlackTexture;
		PTexture                               m_pWhiteTexture;
		PTexture                               m_pDefaultNormalMap;

		CFog                                   m_pCurrentFog;

		PStaticBinder                          m_pStaticBinder;
	};

	typedef base::TSingelton<CRenderManager> TheRenderManager;

	struct SRenderableInfo
	{
		SRenderableInfo();

		math::CFrame*				 pFrame;
		PMaterial					 spMaterial;
		boost::function<void (void)> pRenderFunc;
		boost::function<void (void)> pDebugRenderFunc;
		render::PEffect				 spShader;
		bool						 bHaveVolumes;
		math::AABoxf				 bbox;
		math::Spheref				 bsphere;
	};

	class IRendererable
	{
	public:
		IRendererable(unsigned priority = 1);
		virtual ~IRendererable();

		void			setPriority(unsigned priority) {m_nRenderPriority = priority;}
		unsigned		getPriority() const {return m_nRenderPriority;}

		void			hide() {m_bIsVisible = false;}
		void			show() {m_bIsVisible = true;}

		virtual const SRenderableInfo& getRenderableInfo() const {return m_renderInfo;}
		bool		isVisible()	const {return m_bIsVisible;}

	protected:
		mutable SRenderableInfo	m_renderInfo;
		
	private:
		bool	 m_bIsVisible;
		unsigned m_nRenderPriority;
	};
}