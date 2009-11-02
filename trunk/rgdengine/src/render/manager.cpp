#include "precompiled.h"

#include <rgde/render/manager.h>
#include <rgde/render/binders.h>
#include <rgde/render/light_manager.h>
#include <rgde/render/device.h>
#include <rgde/render/camera_manager.h>

#include <rgde/base/lexical_cast.h>

#include <rgde/core/application.h>

#include <d3dx9.h>
#include "texture_impl.h"


extern LPDIRECT3DDEVICE9 g_d3d;

namespace render
{
	texture_ptr safeLoadDefaultTexture(const std::string &strTextureName)
	{
		io::path_add_scoped p	("Common/");
		texture_ptr pResult	= texture::create(strTextureName);

		if (!pResult)
		{
			base::lerr << "Can't load default texture \"" << strTextureName << "\"";
			core::application::get()->close();
		}

		return pResult;
	}

	render_manager::render_manager()
		: 	m_bLightingEnabled(true),
			m_nFillMode(Solid),
			m_bVolumes(true),
			m_pWhiteTexture(safeLoadDefaultTexture("White.jpg")),
			m_pDefaultNormalMap(safeLoadDefaultTexture("DefaultNormalMap.jpg")),
			m_pBlackTexture(safeLoadDefaultTexture("Black.jpg")),
			m_pDefaultEffect(effect::create("Default.fx")),
			m_pDefaultFont(base_font::create(11,  L"Arial", render::base_font::Heavy))			
	{

		m_pDefaultFog.load_from_xml("Default.xml");
		m_pCurrentFog = m_pDefaultFog;


		if (!m_pDefaultEffect)
		{
			base::lerr << "Can't load effect \"graphics/shaders/Default.fx\"";
			core::application::get()->close();
		}

		if (!m_pDefaultFont)
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
		return m_pDefaultEffect;
	}

	font_ptr& render_manager::getDefaultFont()
	{
		return m_pDefaultFont;
	}

	texture_ptr& render_manager::getBlackTexture()
	{
		return m_pBlackTexture;
	}

	texture_ptr& render_manager::getWhiteTexture()
	{
		return m_pWhiteTexture;
	}

	texture_ptr& render_manager::getDefaultNormalMap()
	{
		return m_pDefaultNormalMap;
	}

	void render_manager::setCurrentFog(const Fog &pFog)
	{
		m_pCurrentFog = pFog;
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
		//void setupParameters(effect_ptr pEffect, const renderable_info &info, PMaterial& mat)
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
			effect_ptr& defaultEffect;
			SDefaultRender() 
				: defaultEffect(TheRenderManager::get().getDefaultEffect())
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
					static PMaterial pDefaultMaterial = material::create();

					//const PMaterial& pMaterial = info.spMaterial ? info.spMaterial : pDefaultMaterial;
					//const effect_ptr&	 pEffect	= info.spShader ? info.spShader : defaultEffect;

					const PMaterial& pMaterial = pDefaultMaterial;
					const effect_ptr&	 pEffect	= defaultEffect;


					//m_pDefaultEffect
					
					pMaterial->getDynamicBinder()->setupParameters(info.frame);
									
					effect::ITechnique *pTechnique = pMaterial->getTechnique();

					if(NULL != pTechnique)
					{	
						pTechnique->begin();
						//base::lmsg << "effect tech -=" << pTechnique->get_name() << "=- begin";

						std::vector<effect::ITechnique::IPass*>   &vecPasses = pTechnique->getPasses();

						for (size_t i = 0; i < vecPasses.size(); i++)
						{
							effect::ITechnique::IPass	*pass = vecPasses[i];
							pass->begin();
								info.render_func();
							pass->end();
						}

						pTechnique->end();
						//base::lmsg << "effect tech <" << pTechnique->get_name() << "> end";
					}
					else
					{
						//info.frame->getFullTransform()
						//return mProj*(mView*frame->getFullTransform());
						g_d3d->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&info.frame->getFullTransform()[0]);
						//g_d3d->Set
						info.render_func();
						//base::lmsg << "Invalid binder or technique";
					}
				}
				else
				{
					info.render_func();
				}
			}

			inline void renderDebug(renderable_info &info)
			{
				if (info.pDebugRenderFunc != NULL)
					info.pDebugRenderFunc();
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
			math::Vec3f	vCamPos;
			SDistanceSorter_Less(const math::Vec3f &camPos)
				: vCamPos(camPos)
			{
			}

			bool operator()(renderable_info const * r1, renderable_info const * r2)
			{
				const math::frame_ptr& pFrame1 = r1->frame;
				const math::frame_ptr& pFrame2 = r2->frame;

				math::Vec3f		pos1, pos2;
				if (pFrame1)
					pos1 = pFrame1->getGlobalPosition();
				if (pFrame2)
					pos2 = pFrame2->getGlobalPosition();
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

			const renderable_info  &ri = r->getRenderableInfo();

			if (ri.frame)
			{
				const math::Point3f& max	= ri.bbox.getMax();
				const math::Point3f& min	= ri.bbox.getMin();
				math::Point3f center		= min + (max - min) / 2.0f;

				float fHalfLenght = math::length<float, 3>(max - min) / 2.0f;

				math::Point3f centerGlobal = ri.frame->getFullTransform() * center;

				if (!m_frustum.CubeInFrustum(centerGlobal[0], centerGlobal[1], centerGlobal[2], fHalfLenght))
					return;
			}

			if (r->get_priority() >= 1000)
				vposttrans.push_back(&ri);
			else if (ri.spMaterial && ri.spMaterial->isTransparent())
				vtrans.push_back(&ri);
			else
				vsolids.push_back(&ri);
		}
	};	

	void render_manager::renderScene()
	{
		render::TheDevice::get().reset_statistics();

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

				if(!m_pStaticBinder)
					createBinder();
				m_pStaticBinder->setupParameters(0);

				const math::frustum& frustum = TheDevice::get().get_camera()->get_frustum();
				std::for_each(m_lRenderables.begin(), m_lRenderables.end(), SRenderblesSorter(vSolid, vTransparet, vPostTransparet, frustum));

				int nVisibleObjects = static_cast<int>(vTransparet.size() + vSolid.size());
				//std::string str = base::Lexical_cast<std::string, int>(nVisibleObjects);
				//std::wstring wstr(str.begin(), str.end());
				//getDefaultFont()->renderText(wstr, math::Rect(1, 29, 400, 400), 0xFFFFFFFF, true);

				std::sort(vTransparet.begin(), vTransparet.end(), functors::SDistanceSorter_Less(TheDevice::get().get_camera()->getPosition()));

				{
					{
						const math::camera_ptr& cam = *camera;
						const math::Matrix44f& mView = cam->get_view_matrix();
						const math::Matrix44f& mProj = cam->get_proj_matrix();
						//return mProj*(mView*frame->getFullTransform());
						g_d3d->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView[0]);
						g_d3d->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProj[0]);
					}


					functors::SDefaultRender r;
					std::for_each(vSolid.begin(),			vSolid.end(),			r);
					std::for_each(vTransparet.begin(),		vTransparet.end(),		r);
					std::for_each(vPostTransparet.begin(),	vPostTransparet.end(),	r);
				}
			}
		}
		else // на случай если нам надо рисовать что-то в экранных координатах и только
		{
			if(!m_pStaticBinder) 
				createBinder();
			m_pStaticBinder->setupParameters(0);

			functors::SDefaultRender r;

			Renderables temp_copy = m_lRenderables;

			for (Renderables::iterator it = temp_copy.begin();
				it != temp_copy.end(); ++it)
			{
				if ((*it)->isVisible())
					r(&(*it)->getRenderableInfo());
			}
		}

		// draw debug information
		//scene::TheScene::get().debug_draw();
		//render::TheDevice::get().showStatistics(getDefaultFont());
	}

	void render_manager::createBinder()
	{
		m_pStaticBinder = createStaticBinder(m_pDefaultEffect);
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
		  bHaveVolumes(false),
		  spMaterial()
	{
	}
}