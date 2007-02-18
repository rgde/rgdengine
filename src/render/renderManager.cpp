#include "precompiled.h"

#include "render/renderManager.h"
#include "render/renderBinders.h"
#include "render/renderLightManager.h"
#include "render/renderDevice.h"
#include "render/renderCameraManager.h"

#include "base/lexical_cast.h"

#include "core/application.h"

#include <d3dx9.h>
#include "renderTextureImpl.h"


namespace render
{
	PTexture safeLoadDefaultTexture(const std::string &strTextureName)
	{
		io::ScopePathAdd p	("Common/");
		PTexture pResult	= ITexture::Create(strTextureName);

		if (!pResult)
		{
			base::lerr << "Can't load default texture \"" << strTextureName << "\"";
			core::IApplication::Get()->close();
		}

		return pResult;
	}

	CRenderManager::CRenderManager()
		: 	m_bLightingEnabled(true),
			m_nFillMode(Solid),
			m_bVolumes(true),
			m_pWhiteTexture(safeLoadDefaultTexture("White.jpg")),
			m_pDefaultNormalMap(safeLoadDefaultTexture("DefaultNormalMap.jpg")),
			m_pBlackTexture(safeLoadDefaultTexture("Black.jpg")),
			m_pDefaultEffect(IEffect::Create("Default.fxo")),
			m_pDefaultFont(IFont::Create(11,  L"Arial", render::IFont::Heavy))			
	{

		m_pDefaultFog.loadFromXML("Default.xml");
		m_pCurrentFog = m_pDefaultFog;


		if (!m_pDefaultEffect)
		{
			base::lerr << "Can't load effect \"graphics/shaders/Default.fxo\"";
			core::IApplication::Get()->close();
		}

		if (!m_pDefaultFont)
		{
			base::lerr << "Can't create font \"" << "Arial" << "\"";
			core::IApplication::Get()->close();
		}
	}

	CRenderManager::~CRenderManager()
	{
	}

	PEffect& CRenderManager::getDefaultEffect()
	{
		return m_pDefaultEffect;
	}

	PFont& CRenderManager::getDefaultFont()
	{
		return m_pDefaultFont;
	}

	PTexture& CRenderManager::getBlackTexture()
	{
		return m_pBlackTexture;
	}

	PTexture& CRenderManager::getWhiteTexture()
	{
		return m_pWhiteTexture;
	}

	PTexture& CRenderManager::getDefaultNormalMap()
	{
		return m_pDefaultNormalMap;
	}

	void CRenderManager::setCurrentFog(const CFog &pFog)
	{
		m_pCurrentFog = pFog;
	}

	void CRenderManager::add(IRendererable *r)
	{
		m_lRenderables.push_back(r);
	}

	void CRenderManager::remove(IRendererable *r)
	{
		//m_lRenderables.remove(r);
		Renderables::iterator it = std::find(m_lRenderables.begin(), m_lRenderables.end(), r);
		m_lRenderables.erase(it);
	}

	void CRenderManager::clear()
	{
		m_lRenderables.resize(0);
	}

	namespace functors
	{
		//void setupParameters(PEffect pEffect, const SRenderableInfo &info, PMaterial& mat)
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
			PEffect& defaultEffect;
			SDefaultRender() 
				: defaultEffect(TheRenderManager::Get().getDefaultEffect())
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
					static PMaterial pDefaultMaterial = CMaterial::Create();

					PMaterial&	pMaterial = info.spMaterial ? info.spMaterial : pDefaultMaterial;
					PEffect&	pEffect	= info.spShader ? info.spShader : defaultEffect;
					
					pMaterial->getDynamicBinder()->setupParameters(info.pFrame);
									
					IEffect::ITechnique *pTechnique = pMaterial->getTechnique();

					if(NULL != pTechnique)
					{	
						pTechnique->begin();
						//base::lmsg << "effect tech -=" << pTechnique->getName() << "=- begin";

						std::vector<IEffect::ITechnique::IPass*>   &vecPasses = pTechnique->getPasses();

						for (size_t i = 0; i < vecPasses.size(); i++)
						{
							IEffect::ITechnique::IPass	*pass = vecPasses[i];
							pass->begin();
								info.pRenderFunc();
							pass->end();
						}

						pTechnique->end();
						//base::lmsg << "effect tech <" << pTechnique->getName() << "> end";
					}
					else
					{
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
			bool operator()(IRendererable *r1, IRendererable *r2)
			{
				return r1->getPriority() < r2->getPriority() ? true : false;
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
				const math::PFrame& pFrame1 = r1->pFrame;
				const math::PFrame& pFrame2 = r2->pFrame;

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
		const math::CFrustum				   &m_frustum;

		SRenderblesSorter(std::vector<SRenderableInfo const *> &solids, std::vector<SRenderableInfo const *> &trans, std::vector<SRenderableInfo const *> &posttrans, const math::CFrustum &frustum)
			: vsolids(solids),
			  vtrans(trans),
			  vposttrans(posttrans),
			  m_frustum(frustum)
		{
		}

		void operator()(IRendererable const * r)
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

			if (r->getPriority() >= 1000)
				vposttrans.push_back(&ri);
			else if (ri.spMaterial && ri.spMaterial->isTransparent())
				vtrans.push_back(&ri);
			else
				vsolids.push_back(&ri);
		}
	};	

	void CRenderManager::renderScene()
	{
		render::TheDevice::Get().resetStats();

		//m_lRenderables.sort(functors::SPrioritySorter_Less());
		std::sort(m_lRenderables.begin(), m_lRenderables.end(), functors::SPrioritySorter_Less());

		TheCameraManager::Get().sortCameras();

		static std::vector<SRenderableInfo const *> vPostTransparet(1000);
		static std::vector<SRenderableInfo const *> vTransparet(1000);
		static std::vector<SRenderableInfo const *> vSolid(1000);

		// draw scene through every active camera
		CameraManager &cm	= TheCameraManager::Get();
		if (cm.begin() != cm.end())
		{
			for (CameraManager::CameraListIterator camera = cm.begin(); camera != cm.end(); ++camera)
			{
				vSolid.resize(0);
				vTransparet.resize(0);
				vPostTransparet.resize(0);

				TheCameraManager::Get().setCamera(camera);

				if(!m_pStaticBinder)
					createBinder();
				m_pStaticBinder->setupParameters(0);

				const math::CFrustum& frustum = TheDevice::Get().getCurentCamera()->getFrustum();
				std::for_each(m_lRenderables.begin(), m_lRenderables.end(), SRenderblesSorter(vSolid, vTransparet, vPostTransparet, frustum));

				int nVisibleObjects = static_cast<int>(vTransparet.size() + vSolid.size());
				//std::string str = base::Lexical_cast<std::string, int>(nVisibleObjects);
				//std::wstring wstr(str.begin(), str.end());
				//getDefaultFont()->renderText(wstr, math::Rect(1, 29, 400, 400), 0xFFFFFFFF, true);

				std::sort(vTransparet.begin(), vTransparet.end(), functors::SDistanceSorter_Less(TheDevice::Get().getCurentCamera()->getPosition()));

				{
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

			for (Renderables::iterator it = m_lRenderables.begin();
				it != m_lRenderables.end(); ++it)
			{
				if ((*it)->isVisible())
					r(&(*it)->getRenderableInfo());
			}
		}

		// draw debug information
		//scene::TheScene::Get().debugDraw();
		//render::TheDevice::Get().showStatistics(getDefaultFont());
	}

	void CRenderManager::createBinder()
	{
		m_pStaticBinder = createStaticBinder(m_pDefaultEffect);
	}

	IRendererable::IRendererable(unsigned priority)
		: m_nRenderPriority(priority),
		  m_bIsVisible(true)
	{
		TheRenderManager::Get().add(this);
	}

	IRendererable::~IRendererable()
	{
		if (TheRenderManager::IsCreated())
			TheRenderManager::Get().remove(this);
	}


	SRenderableInfo::SRenderableInfo()
		: pFrame(0),
		  bHaveVolumes(false),
		  spMaterial(CMaterial::Create())
	{
	}
}