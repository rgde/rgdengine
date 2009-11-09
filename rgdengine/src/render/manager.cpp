#include "precompiled.h"

#include <rgde/render/manager.h>
#include <rgde/render/binders.h>
#include <rgde/render/light_manager.h>
#include <rgde/render/render_device.h>
#include <rgde/render/camera_manager.h>

#include <rgde/base/lexical_cast.h>

#include <rgde/core/application.h>

#include <d3dx9.h>
#include "texture_impl.h"


extern LPDIRECT3DDEVICE9 g_d3d;

namespace render
{
	namespace
	{
		texture_ptr load_default_texture(const std::string &texture_name)
		{
			io::path_add_scoped p	("Common/");
			texture_ptr tex	= texture::create(texture_name);

			if (!tex)
			{
				base::lerr << "Can't load default texture \"" << texture_name << "\"";
				core::application::get()->close();
			}

			return tex;
		}
	}

	render_manager::render_manager()
		: m_volumes(true)
		, m_white_texture(load_default_texture("White.jpg"))
		, m_flat_normal_texture(load_default_texture("DefaultNormalMap.jpg"))
		, m_black_texture(load_default_texture("Black.jpg"))
		, m_default_sffect(effect::create("Default.fx"))
		, m_default_font(font::create(11,  L"Arial", render::font::Heavy))
	{

		if (!m_default_sffect)
		{
			base::lerr << "Can't load effect \"graphics/shaders/Default.fx\"";
			core::application::get()->close();
		}

		if (!m_default_font)
		{
			base::lerr << "Can't create font \"" << "Arial" << "\"";
			core::application::get()->close();
		}
	}

	render_manager::~render_manager()
	{
	}

	effect_ptr& render_manager::getDefaultEffect()
	{
		return m_default_sffect;
	}

	font_ptr& render_manager::getDefaultFont()
	{
		return m_default_font;
	}

	texture_ptr& render_manager::getBlackTexture()
	{
		return m_black_texture;
	}

	texture_ptr& render_manager::getWhiteTexture()
	{
		return m_white_texture;
	}

	texture_ptr& render_manager::getDefaultNormalMap()
	{
		return m_flat_normal_texture;
	}

	void render_manager::add(rendererable *r)
	{
		m_lRenderables.push_back(r);
	}

	void render_manager::remove(rendererable *r)
	{
		//m_lRenderables.remove(r);
		Renderables::iterator it = std::find(m_lRenderables.begin(), m_lRenderables.end(), r);
		m_lRenderables.erase(it);
	}

	void render_manager::clear()
	{
		m_lRenderables.resize(0);
	}

	namespace functors
	{
		//void setupParameters(effect_ptr effect, const renderable_info &info, material_ptr& mat)
		//{
		//	//assert(info.frame);
		//	if (info.frame)
		//	{
		//		m_woldMatrixBinder.setFrame(*(info.frame));
		//		m_woldMatrixBinder.setupParameters();		
		//		mat->bind();
		//	}
		//	
		//}

		struct SDefaultRender
		{			
			render_manager& m_manager;

			SDefaultRender(render_manager* manager)
				: m_manager(*manager)
			{
			}

			void operator()(renderable_info const * r)
			{
				if (NULL == r) return;
				render(*r);
			}

			inline void render(const renderable_info &info)
			{
				if(info.frame)
				{
					//const material_ptr& pMaterial = info.material ? info.material : pDefaultMaterial;
					//const effect_ptr&	 effect	= info.shader ? info.shader : defaultEffect;

					const material_ptr mat = m_manager.get_default_material();
					const effect_ptr&  effect	= m_manager.getDefaultEffect();
				
					mat->getDynamicBinder()->setupParameters(info.frame);
									
					effect::technique *pTechnique = mat->getTechnique();

					if(NULL != pTechnique)
					{	
						pTechnique->begin();
						//base::lmsg << "effect tech -=" << pTechnique->get_name() << "=- begin";

						std::vector<effect::technique::pass*>   &vecPasses = pTechnique->get_passes();

						for (size_t i = 0; i < vecPasses.size(); i++)
						{
							effect::technique::pass	*pass = vecPasses[i];
							pass->begin();
								info.render_func();
							pass->end();
						}

						pTechnique->end();
						//base::lmsg << "effect tech <" << pTechnique->get_name() << "> end";
					}
					else
					{
						//info.frame->get_full_tm()						
						g_d3d->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&info.frame->get_full_tm()[0]);						
						info.render_func();
					}
				}
				else
				{
					info.render_func();
				}
			}

			inline void renderDebug(renderable_info &info)
			{
				if (info.debug_render_func != NULL)
					info.debug_render_func();
			}
		};

		struct priority_sorter_less
		{
			bool operator()(rendererable *r1, rendererable *r2)
			{
				return r1->get_priority() < r2->get_priority() ? true : false;
			}
		};

		struct SDistanceSorter_Less
		{
			math::vec3f	vCamPos;
			SDistanceSorter_Less(const math::vec3f &camPos)
				: vCamPos(camPos)
			{
			}

			bool operator()(renderable_info const * r1, renderable_info const * r2)
			{
				const math::frame_ptr& pFrame1 = r1->frame;
				const math::frame_ptr& pFrame2 = r2->frame;

				math::vec3f		pos1, pos2;
				if (pFrame1)
					pos1 = pFrame1->get_world_pos();
				if (pFrame2)
					pos2 = pFrame2->get_world_pos();
				float	fLengthSqr1			= math::lengthSquared<float, 3>(vCamPos - pos1),
						fLengthSqr2			= math::lengthSquared<float, 3>(vCamPos - pos2);

				return fLengthSqr1 > fLengthSqr2;
			}
		};
	}

	struct SRenderblesSorter
	{
		std::vector<renderable_info const *>   &vsolids;
		std::vector<renderable_info const *>   &vtrans;
		std::vector<renderable_info const *>   &vposttrans;
		const math::frustum				   &m_frustum;

		SRenderblesSorter(std::vector<renderable_info const *> &solids, std::vector<renderable_info const *> &trans, std::vector<renderable_info const *> &posttrans, const math::frustum &frustum)
			: vsolids(solids),
			  vtrans(trans),
			  vposttrans(posttrans),
			  m_frustum(frustum)
		{
		}

		void operator()(rendererable const * r)
		{
			if ((NULL == r) || (r->isVisible() == false))
				return;

			const renderable_info  &ri = r->get_renderable_info();

			if (ri.frame)
			{
				const math::point3f& max	= ri.bbox.getMax();
				const math::point3f& min	= ri.bbox.getMin();
				math::point3f center		= min + (max - min) / 2.0f;

				float fHalfLenght = math::length<float, 3>(max - min) / 2.0f;

				math::point3f centerGlobal = ri.frame->get_full_tm() * center;

				// skip test on that low level.
				//if (!m_frustum.test_sphere(centerGlobal[0], centerGlobal[1], centerGlobal[2], fHalfLenght))
				//	return;
			}

			if (r->get_priority() >= 1000)
				vposttrans.push_back(&ri);
			else if (ri.material && ri.material->isTransparent())
				vtrans.push_back(&ri);
			else
				vsolids.push_back(&ri);
		}
	};	

	void render_manager::renderScene()
	{
		render::render_device::get().reset_statistics();

		//m_lRenderables.sort(functors::priority_sorter_less());
		std::sort(m_lRenderables.begin(), m_lRenderables.end(), functors::priority_sorter_less());

		TheCameraManager::get().sort();

		static std::vector<renderable_info const *> vPostTransparet(1000);
		static std::vector<renderable_info const *> vTransparet(1000);
		static std::vector<renderable_info const *> vSolid(1000);

		// draw scene through every active camera
		camera_manager &cm	= TheCameraManager::get();
		if (cm.begin() != cm.end())
		{
			for (camera_manager::CameraListIterator camera = cm.begin(); camera != cm.end(); ++camera)
			{
				vSolid.resize(0);
				vTransparet.resize(0);
				vPostTransparet.resize(0);

				TheCameraManager::get().set_camera(camera);

				if(!m_static_binder)
					createBinder();
				m_static_binder->setupParameters(0);

				const math::frustum& frustum = render_device::get().get_camera()->get_frustum();
				std::for_each(m_lRenderables.begin(), m_lRenderables.end(), SRenderblesSorter(vSolid, vTransparet, vPostTransparet, frustum));

				int nVisibleObjects = static_cast<int>(vTransparet.size() + vSolid.size());
				//std::string str = base::Lexical_cast<std::string, int>(nVisibleObjects);
				//std::wstring wstr(str.begin(), str.end());
				//getDefaultFont()->renderText(wstr, math::Rect(1, 29, 400, 400), 0xFFFFFFFF, true);

				std::sort(vTransparet.begin(), vTransparet.end(), functors::SDistanceSorter_Less(render_device::get().get_camera()->get_pos()));

				{
					{
						const math::camera_ptr& cam = *camera;
						const math::matrix44f& mView = cam->get_view_matrix();
						const math::matrix44f& mProj = cam->get_proj_matrix();
						//return mProj*(mView*frame->get_full_tm());
						g_d3d->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView[0]);
						g_d3d->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProj[0]);
					}


					functors::SDefaultRender r(this);
					std::for_each(vSolid.begin(),			vSolid.end(),			r);
					std::for_each(vTransparet.begin(),		vTransparet.end(),		r);
					std::for_each(vPostTransparet.begin(),	vPostTransparet.end(),	r);
				}
			}
		}
		else // на случай если нам надо рисовать что-то в экранных координатах и только
		{
			if(!m_static_binder) 
				createBinder();
			m_static_binder->setupParameters(0);

			functors::SDefaultRender r(this);

			Renderables temp_copy = m_lRenderables;

			for (Renderables::iterator it = temp_copy.begin();
				it != temp_copy.end(); ++it)
			{
				if ((*it)->isVisible())
					r(&(*it)->get_renderable_info());
			}
		}

		// draw debug information
		//scene::TheScene::get().debug_draw();
		//render::render_device::get().showStatistics(getDefaultFont());
	}

	void render_manager::createBinder()
	{
		m_static_binder = createStaticBinder(m_default_sffect);
	}

	material_ptr render_manager::get_default_material() 
	{
		if (!m_default_material)
			m_default_material = material::create();

		return m_default_material;
	}

	rendererable::rendererable(unsigned priority)
		: m_nRenderPriority(priority),
		  m_is_visible(true)
	{
		TheRenderManager::get().add(this);
	}

	rendererable::~rendererable()
	{
		if (TheRenderManager::is_created())
			TheRenderManager::get().remove(this);
	}


	renderable_info::renderable_info()
		: frame(0),
		  has_volumes(false),
		  material()
	{
	}
}