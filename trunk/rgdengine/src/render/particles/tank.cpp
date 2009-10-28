#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/tank.h>


namespace particles
{

	//-----------------------------------------------------------------------------------
	renderer::renderer() : m_nReservedSize(0)
	{
		m_spEffect = render::effect::create( "particles.fxo" );

		render::effect::Parameters& params = m_spEffect->getParams();

		m_paramUpVec			= params["m_vUp"];
		m_paramRightVec			= params["m_vRight"];
		m_paramParticleTexture	= params["ParticlesTexture"];
		m_paramTransformMatrix	= params["m_mLVP"];

		m_pRenderTechnique = m_spEffect->findTechnique("PartilesRenderModulate");

		setTextureTiling(1, 1, 1);
	}

	//-----------------------------------------------------------------------------------
	renderer::~renderer()
	{
	}
	//-----------------------------------------------------------------------------------
	void renderer::render(render::texture_ptr texture, math::Frame& frame)
	{
		if( (m_nReservedSize == 0) )
			return;

		const math::Matrix44f& mLocal	= frame.getFullTransform();
		math::camera_ptr pCamera		= render::TheDevice::get().get_curent_camera();
		const math::Matrix44f& mProj	= pCamera->getProjMatrix();
		const math::Matrix44f& mView	= pCamera->getViewMatrix();
		
		math::Matrix44f mSystem = mView * mLocal;
		math::Matrix44f mLVP = mProj * mSystem;

		bool useGlobalUp = true;

		math::Vec4f right(mSystem.mData[0], mSystem.mData[4], mSystem.mData[8], 0.0f);
		//math::normalize(right);
		math::Vec4f up(mSystem.mData[1], mSystem.mData[5], mSystem.mData[9], 0);
		//math::normalize(up);
	

		if (useGlobalUp)
		{
			right[2] = 0;
			math::normalize(right);
			up = math::Vec4f(0, 0, 1, 0);
		}


		m_paramParticleTexture->set(texture);
		m_paramTransformMatrix->set(mLVP);
		m_paramRightVec->set(right);
		m_paramUpVec->set(up);

		
		std::vector<render::effect::ITechnique::IPass*>& passes = m_pRenderTechnique->getPasses();

		m_spEffect->commitChanges();
		m_pRenderTechnique->begin();

		for(size_t pass = 0; pass < passes.size(); ++pass)
		{
			passes[pass]->begin();
			m_Geometry.render( render::PrimTypeTriangleList, 2*(unsigned)m_vParticleArray.size() );
			passes[pass]->end();
		}

		m_pRenderTechnique->end();
	}
	//-----------------------------------------------------------------------------------
	void renderer::update()
	{
		unsigned int nParticles = (unsigned int)m_vParticleArray.size();

		if( nParticles == 0 ) return;

		if( (nParticles > m_nReservedSize) )
		{
			m_nReservedSize = nParticles;

			Geometry::Indexes& vIndices = m_Geometry.lockIB();
			if( vIndices.size() < nParticles*6 )
				vIndices.resize(nParticles*6);
			for(unsigned i = 0; i < nParticles; ++i)
			{
				vIndices[i * 6 + 0] = i * 4 + 0;
				vIndices[i * 6 + 1] = i * 4 + 1;
				vIndices[i * 6 + 2] = i * 4 + 2;
				vIndices[i * 6 + 3] = i * 4 + 3;
				vIndices[i * 6 + 4] = i * 4 + 2;
				vIndices[i * 6 + 5] = i * 4 + 1;
			}
			m_Geometry.unlockIB();
		}

		Geometry::Vertexes& vVertices = m_Geometry.lockVB();
		if( vVertices.size() < nParticles*4 )
			vVertices.resize(nParticles*4);

		unsigned int j = 0;
		for( unsigned int i = 0; i < nParticles; ++i )
		{
			SParticle & p = m_vParticleArray[i];
			float sizex = p.size[0]*0.5f;
			float sizey = p.size[1]*0.5f;
			float& spin = p.spin;

			float xsin = sizex*::sin(spin);
			float xcos = sizex*::cos(spin);
			float ysin = sizey*::sin(spin);
			float ycos = sizey*::cos(spin);

			float fTileY = (float)(p.nTile%m_nRows),
				  fTileX = ((float)p.nTile - fTileY)*m_fInvRows;


			vVertices[j].position = p.pos;
			vVertices[j].tex1 = math::Vec2f( -xcos - ysin, -xsin + ycos );
			vVertices[j].tex0 = math::Vec2f(fTileX*m_fInvTotalColumns, fTileY*m_fInvTotalRows);
			vVertices[j].color = p.color;
			++j;

			vVertices[j].position = p.pos;
			vVertices[j].tex1 = math::Vec2f( xcos - ysin, xsin + ycos );
			vVertices[j].tex0 = math::Vec2f((fTileX + 1.0f)*m_fInvTotalColumns, fTileY*m_fInvTotalRows);
			vVertices[j].color = p.color;
			++j;

			vVertices[j].position = p.pos;
			vVertices[j].tex1 = math::Vec2f( -xcos + ysin, -xsin - ycos );
			vVertices[j].tex0 = math::Vec2f(fTileX*m_fInvTotalColumns, (fTileY + 1.0f)*m_fInvTotalRows);
			vVertices[j].color = p.color;
			++j;

			vVertices[j].position = p.pos;
			vVertices[j].tex1 = math::Vec2f( xcos + ysin, xsin - ycos );
			vVertices[j].tex0 = math::Vec2f((fTileX + 1.0f)*m_fInvTotalColumns, (fTileY + 1.0f)*m_fInvTotalRows);
			vVertices[j].color = p.color;
			++j;
		}
		m_Geometry.unlockVB();
	}

	void renderer::setTextureTiling(int nRows, int nColumnsTotal, int nRowsTotal)
	{
		m_nRows = nRows;
		m_fInvRows = 1.0f/(float)nRows;
		m_fInvTotalColumns = 1.0f/(float)nColumnsTotal;
		m_fInvTotalRows = 1.0f/(float)nRowsTotal;
	}

}