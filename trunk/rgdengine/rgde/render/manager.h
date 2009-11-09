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
		
		void enableVolumes( bool flag )		{ m_volumes = flag; }
		bool isVolumeDrawing() const		{ return m_volumes; }

		void enableLighting(bool bLightingEnabled)  { m_lighting_enabled = bLightingEnabled; }
		bool isLightingEnabled() const		{ return m_lighting_enabled; }

		void setFillMode(FillMode mode)		{ m_fill_mode = mode; }
		FillMode  getFillMode() const			{ return m_fill_mode; }

		effect_ptr& getDefaultEffect();
		font_ptr&   getDefaultFont();
		const Fog&    getDefaultFog()	const {return m_default_fog;}

		texture_ptr& getBlackTexture();
		texture_ptr& getWhiteTexture();
		texture_ptr& getDefaultNormalMap();

		material_ptr get_default_material();

		void setCurrentFog(const Fog& fog);
		const Fog& getCurrentFog() const	{return m_current_fog;}

	private:
		void add(rendererable* r);
		void remove(rendererable* r);

		void createBinder();

	protected:
		typedef std::vector<rendererable*> Renderables;
		Renderables m_lRenderables;

		bool			  m_volumes;
		bool              m_lighting_enabled;
		FillMode          m_fill_mode;

		effect_ptr        m_default_sffect;
		font_ptr          m_default_font;
		Fog               m_default_fog;

		texture_ptr       m_black_texture;
		texture_ptr       m_white_texture;
		texture_ptr       m_flat_normal_texture;

		Fog               m_current_fog;

		material_ptr	  m_default_material;

		static_binder_ptr     m_static_binder;
	};

	typedef base::singelton<render_manager> TheRenderManager;

	struct renderable_info
	{
		renderable_info();

		math::frame*				 frame;
		material_ptr				 material;
		boost::function<void (void)> render_func;
		boost::function<void (void)> debug_render_func;
		render::effect_ptr			 shader;
		bool						 has_volumes;
		math::aaboxf				 bbox;
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

		virtual const renderable_info& get_renderable_info() const {return m_renderInfo;}
		bool		isVisible()	const {return m_is_visible;}

	protected:
		mutable renderable_info	m_renderInfo;
		
	private:
		bool	 m_is_visible;
		unsigned m_nRenderPriority;
	};
}