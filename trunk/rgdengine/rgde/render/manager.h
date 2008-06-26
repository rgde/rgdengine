#pragma once

#include <rgde/math/transform.h>

#include <rgde/render/texture.h>
#include <rgde/render/font.h>
#include <rgde/render/effect.h>
#include <rgde/render/fog.h>
#include <rgde/render/material.h>
#include <rgde/render/binders.h>

namespace render
{
	enum FillMode 
	{
		Point               = 1,
		WireFrame           = 2,
		Solid               = 3,
		FillMode_OutOfRange = 0x7fffffff, /* force 32-bit size enum */
	};

	class Mesh;

	class RenderManager
	{
		friend class IRendererable;

	protected:
		RenderManager();
		virtual ~RenderManager();

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
		const Fog&    getDefaultFog()	const {return m_pDefaultFog;}

		PTexture& getBlackTexture();
		PTexture& getWhiteTexture();
		PTexture& getDefaultNormalMap();

		void setCurrentFog(const Fog& fog);
		const Fog& getCurrentFog() const	{return m_pCurrentFog;}

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
		Fog                                   m_pDefaultFog;

		PTexture                               m_pBlackTexture;
		PTexture                               m_pWhiteTexture;
		PTexture                               m_pDefaultNormalMap;

		Fog                                   m_pCurrentFog;

		PStaticBinder                          m_pStaticBinder;
	};

	typedef base::TSingelton<RenderManager> TheRenderManager;

	struct SRenderableInfo
	{
		SRenderableInfo();

		math::Frame*				 pFrame;
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