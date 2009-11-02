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


extern LPDIRECT3DDEVICE9 g_pd3dDevice;

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
			m_pDefaultFont(IFont::create(11,  L"Arial", render::IFont::Heavy))			
	{

		m_pDefaultFog.loadFromXML("Default.xml");
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
		//void setupParameters(effect_ptr pEffect, const SRenderableInfo &info, PMaterial& mat)
		//{
		//	//assert(info.pFrame);
		//	if (info.pFrame)
		//	{
		//		m_woldMatrixBinder.setFrame(*(info.pFrame));
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

			void operator()(SRenderableInfo const * r)
			{
				if (NULL == r) return;
				render(*r);
			}

			inline void render(const SRenderableInfo &info)
			{
				if(info.pFrame)
				{
					static PMaterial pDefaultMaterial = material::create();

					//const PMaterial& pMaterial = info.spMaterial ? info.spMaterial : pDefaultMaterial;
					//const effect_ptr&	 pEffect	= info.spShader ? info.spShader : defaultEffect;

					const PMaterial& pMaterial = pDefaultMaterial;
					const effect_ptr&	 pEffect	= defaultEffect;


					//m_pDefaultEffect
					
					pMaterial->getDynamicBinder()->setupParameters(info.pFrame);
									
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
								info.pRenderFunc();
							pass->end();
						}

						pTechnique->end();
						//base::lmsg << "effect tech <" << pTechnique->get_name() << "> end";
					}
					else
					{
						//info.pFrame->getFullTransform()
						//return mProj*(mView*frame->getFullTransform());
						g_pd3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&info.pFrame->getFullTransform()[0]);
						//g_pd3dDevice->Set
						info.pRenderFunc();
						//base::lmsg << "Invalid binder or technique";
					}
				}
				else
				{
					info.pRenderFunc();
				}
			}

			inline void renderDebug(SRenderableInfo &info)
			{
				if (info.pDebugRenderFunc != NULL)
					info.pDebugRenderFunc();
			}
		};

		struct SPrioritySorter_Less
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

			bool operator()(SRenderableInfo const * r1, SRenderableInfo const * r2)
			{
				const math::frame_ptr& pFrame1 = r1->pFrame;
				const math::frame_ptr& pFrame2 = r2->pFrame;

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
		std::vector<SRenderableInfo const *>   &vsolids;
		std::vector<SRenderableInfo const *>   &vtrans;
		std::vector<SRenderableInfo const *>   &vposttrans;
		const math::frustum				   &m_frustum;

		SRenderblesSorter(std::vector<SRenderableInfo const *> &solids, std::vector<SRenderableInfo const *> &trans, std::vector<SRenderableInfo const *> &posttrans, const math::frustum &frustum)
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

			const SRenderableInfo  &ri = r->getRenderableInfo();

			if (ri.pFrame)
			{
				const math::Point3f& max	= ri.bbox.getMax();
				const math::Point3f& min	= ri.bbox.getMin();
				math::Point3f center		= min + (max - min) / 2.0f;

				float fHalfLenght = math::length<float, 3>(max - min) / 2.0f;

				math::Point3f centerGlobal = ri.pFrame->getFullTransform() * center;

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
		render::TheDevice::get().resetStats();

		//m_lRenderables.sort(functors::SPrioritySorter_Less());
		std::sort(m_lRenderables.begin(), m_lRenderables.end(), functors::SPrioritySorter_Less());

		TheCameraManager::get().sortCameras();

		static std::vector<SRenderableInfo const *> vPostTransparet(1000);
		static std::vector<SRenderableInfo const *> vTransparet(1000);
		static std::vector<SRenderableInfo const *> vSolid(1000);

		// draw scene through every active camera
		CameraManager &cm	= TheCameraManager::get();
		if (cm.begin() != cm.end())
		{
			for (CameraManager::CameraListIterator camera = cm.begin(); camera != cm.end(); ++camera)
			{
				vSolid.resize(0);
				vTransparet.resize(0);
				vPostTransparet.resize(0);

				TheCameraManager::get().setCamera(camera);

				if(!m_pStaticBinder)
					createBinder();
				m_pStaticBinder->setupParameters(0);

				const math::frustum& frustum = TheDevice::get().get_curent_camera()->getFrustum();
				std::for_each(m_lRenderables.begin(), m_lRenderables.end(), SRenderblesSorter(vSolid, vTransparet, vPostTransparet, frustum));

				int nVisibleObjects = static_cast<int>(vTransparet.size() + vSolid.size());
				//std::string str = base::Lexical_cast<std::string, int>(nVisibleObjects);
				//std::wstring wstr(str.begin(), str.end());
				//getDefaultFont()->renderText(wstr, math::Rect(1, 29, 400, 400), 0xFFFFFFFF, true);

				std::sort(vTransparet.begin(), vTransparet.end(), functors::SDistanceSorter_Less(TheDevice::get().get_curent_camera()->getPosition()));

				{
					{
						const math::camera_ptr& pCamera = *camera;
						const math::Matrix44f& mView = pCamera->getViewMatrix();
						const math::Matrix44f& mProj = pCamera->getProjMatrix();
						//return mProj*(mView*frame->getFullTransform());
						g_pd3dDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView[0]);
						g_pd3dDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProj[0]);
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
		//scene::TheScene::get().debugDraw();
		//render::TheDevice::get().showStatistics(getDefaultFont());
	}

	void render_manager::createBinder()
	{
		m_pStaticBinder = createStaticBinder(m_pDefaultEffect);
	}

	rendererable::rendererable(unsigned priority)
		: m_nRenderPriority(priority),
		  m_bIsVisible(true)
	{
		TheRenderManager::get().add(this);
	}

	rendererable::~rendererable()
	{
		if (TheRenderManager::is_created())
			TheRenderManager::get().remove(this);
	}


	SRenderableInfo::SRenderableInfo()
		: pFrame(0),
		  bHaveVolumes(false),
		  spMaterial()
	{
	}
}