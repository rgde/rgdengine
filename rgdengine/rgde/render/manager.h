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

	class mesh;

	class render_manager
	{
		friend class rendererable;

	protected:
		render_manager();
		virtual ~render_manager();

	public:
		void clear();
		void renderScene();
		
		void enableVolumes( bool flag )		{ m_bVolumes = flag; }
		bool isVolumeDrawing() const		{ return m_bVolumes; }

		void enableLighting(bool bLightingEnabled)  { m_bLightingEnabled = bLightingEnabled; }
		bool isLightingEnabled() const		{ return m_bLightingEnabled; }

		void setFillMode(FillMode mode)		{ m_nFillMode = mode; }
		FillMode  getFillMode() const			{ return m_nFillMode; }

		effect_ptr& getDefaultEffect();
		font_ptr&   getDefaultFont();
		const Fog&    getDefaultFog()	const {return m_pDefaultFog;}

		texture_ptr& getBlackTexture();
		texture_ptr& getWhiteTexture();
		texture_ptr& getDefaultNormalMap();

		void setCurrentFog(const Fog& fog);
		const Fog& getCurrentFog() const	{return m_pCurrentFog;}

	private:
		void add(rendererable* r);
		void remove(rendererable* r);

		void createBinder();

	protected:
		typedef std::vector<rendererable*> Renderables;
		Renderables m_lRenderables;
		//std::list<rendererable*>	           m_lRenderables;
		bool						           m_bVolumes;
		bool                                   m_bLightingEnabled;
		FillMode                               m_nFillMode;

		effect_ptr                                m_pDefaultEffect;
		font_ptr                                  m_pDefaultFont;
		Fog                                   m_pDefaultFog;

		texture_ptr                               m_pBlackTexture;
		texture_ptr                               m_pWhiteTexture;
		texture_ptr                               m_pDefaultNormalMap;

		Fog                                   m_pCurrentFog;

		PStaticBinder                          m_pStaticBinder;
	};

	typedef base::singelton<render_manager> TheRenderManager;

	struct renderable_info
	{
		renderable_info();

		math::frame*				 frame;
		material_ptr					 spMaterial;
		boost::function<void (void)> render_func;
		boost::function<void (void)> pDebugRenderFunc;
		render::effect_ptr				 spShader;
		bool						 bHaveVolumes;
		math::AABoxf				 bbox;
		math::Spheref				 bsphere;
	};

	class rendererable
	{
	public:
		rendererable(unsigned priority = 1);
		virtual ~rendererable();

		void			set_priority(unsigned priority) {m_nRenderPriority = priority;}
		unsigned		get_priority() const {return m_nRenderPriority;}

		void			hide() {m_is_visible = false;}
		void			show() {m_is_visible = true;}

		virtual const renderable_info& getRenderableInfo() const {return m_renderInfo;}
		bool		isVisible()	const {return m_is_visible;}

	protected:
		mutable renderable_info	m_renderInfo;
		
	private:
		bool	 m_is_visible;
		unsigned m_nRenderPriority;
	};
}