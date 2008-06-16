#include "precompiled.h"

#include <rgde/render/particles/particlesMain.h>
#include <rgde/render/particles/particlesMayaEmitter.h>
#include <rgde/render/particles/particlesParticleTank.h>

using namespace particles::MayaStructs;

namespace particles{

	std::map< std::string, MayaStructs::SFrameSequence> IMayaEmitter::m_FrSeq;
	IMayaEmitter::PFrames IMayaEmitter::ms_PFrames;

	static std::wstring ms_strBaseFolder;

	int em_num = 0;

	//-----------------------------------------------------------------------------------
	void IMayaEmitter::ClearCachedData()
	{
		ms_PFrames.clear();
		m_FrSeq.clear();

		em_num = 0;			// zero emitters counter
	}
	//-----------------------------------------------------------------------------------
	IMayaEmitter::IMayaEmitter(const std::string& sequence_name, const std::string texture_name) 
		: m_fScale(1.0f), IEmitter(IEmitter::Maya)
	{
		m_bIntense				= false;

		m_bIsSeqLoaded			= false;
		m_bIsTexLoaded			= false;

		m_bIsFading				= false;

		m_is_visible				= false;
		m_bCycling				= false;

		m_time_shift			= 0;

		m_fLastFrame			= 0;
		m_fLastTime				= -1.0f;

		m_bIsAnimTextureUsed	= false;
		m_cTexFps				= 25;

		m_TexName				= texture_name;

		loadTexture();
		loadFrames(sequence_name);

		// public properties:
		//REGISTER_PROPERTY(bIsFading, bool)
		//REGISTER_PROPERTY(fTimeShift, int)
		//REGISTER_PROPERTY(bVisible, bool)
		//REGISTER_PROPERTY(bCycling, bool)
		//REGISTER_PROPERTY(bIntense, bool)
		//REGISTER_PROPERTY(TexName, std::string)
	}
	//-----------------------------------------------------------------------------------
	IMayaEmitter::~IMayaEmitter()
	{
	}
	//-----------------------------------------------------------------------------------
	void IMayaEmitter::loadTexture()
	{
		//try{
		//	std::map<std::string, agl::AGLTexture>::iterator it = ms_Textures.find(m_TexName);
		//	if (ms_Textures.end() == it)
		//	{
		//		agl::LocalImgPathSync p(pfx::tex_path.c_str());

		//		try
		//		{
		//			m_texture.load(m_TexName);
		//			m_texture.setFilterMode(agl::filter_Linear);
		//			ms_Textures[m_TexName]  = m_texture;
		//		}
		//		catch (...)
		//		{
		//		}                      
		//	}
		//	else
		//	{
		//		m_texture = it->second;
		//	}

		//	std::string ext_path = std::string::Concat(
		//		pfx::tex_path.c_str(), pfx::tex_path.length(),
		//		m_TexName.c_str(), m_TexName.length() - 3);
		//	ext_path += "tnf";

		//	if (base::ResourceMaster::Get()->isResourceExist(ext_path))
		//	{
		//		clx::rstream in(base::ResourceMaster::Get()->getResource(ext_path));//ResourseMaster

		//		m_bIsAnimTextureUsed = true;

		//		in  >> m_ucRow			// = 4;
		//			>> m_ucCol			// = 4;
		//			>> m_ucTexFrames		// = 16;
		//			>> m_ucTrow
		//			>> m_ucTcol;

		//		if (m_bIsSeqLoaded) m_bIsSeqLoaded = false;
		//	}
		//	else 
		//		m_bIsAnimTextureUsed = false;
		//}

		m_texture = render::ITexture::Create(m_TexName);// std::wstring(m_TexName.begin(), m_TexName.end()) );
		m_bIsTexLoaded = true;
	}
	//-----------------------------------------------------------------------------------
	void IMayaEmitter::update(float dt)
	{
		if (!m_bIsSeqLoaded) return;

		m_fLastFrame += dt * m_cTexFps;//25.0f;

		if (!(m_fLastFrame < m_time_shift + m_Frames->size()))
		{
			if (m_bCycling) 
			{
				m_fLastFrame  = 0;
				m_fLastTime = dt;
			}
			else
			{
				m_is_visible = false;
				return;
			}
		}
	}
	//-----------------------------------------------------------------------------------
	void IMayaEmitter::render()
	{
		if (!(m_is_visible && m_bIsSeqLoaded))
			return;

		int frame_to_render = (int)(m_fLastFrame - m_time_shift);
		if (frame_to_render < 0) return;

		render((unsigned int)frame_to_render);
	}

	//-----------------------------------------------------------------------------------
	void IMayaEmitter::loadFrames(std::string file_name)
	{
		m_fLastFrame = 0;
		m_fLastTime = -1.0f;	
		{
			if (!m_bIsTexLoaded){
				m_Name.clear();
				return;
			}

			bool just_reload = false;

			// повторно грузить то что уже было не надо :)
			if (m_Name == file_name) 
			{
				if (m_bIsSeqLoaded)
					return;
				else
					just_reload = true;
			}

			PFramesIter it = ms_PFrames.find(file_name);
			m_Name = file_name;
		
			if (it != ms_PFrames.end())
				if (!just_reload)
				{
					m_bIsSeqLoaded = true;
					m_Frames = &(ms_PFrames[m_Name]);
					return;
				}
				else 
					ms_PFrames.erase(file_name);
		}

		{
			SFrameSequence& fseq = m_FrSeq[m_Name];
		
			{
				std::string str = "Media/" + m_Name;
				io::CReadFileStream pfxFile(str);
				if(pfxFile.isOpened())
				{
					if (!fseq.Load(pfxFile))
					{
						m_bIsSeqLoaded = false;
						return;
					}
				}
				else
				{
					//assert(false);
					return;
				}
			}

			PTanks& psyst = ms_PFrames[m_Name];
			psyst.resize(fseq.frames.size());
			

			for (unsigned int i = 0; i < fseq.frames.size(); ++i)
			{
				SFrame& m_spFrame = fseq.frames[i];
				PPTank spTank(new PTank);
				psyst[i] = spTank;

				PTank::ParticleArray& array = spTank->getParticles();

				array.resize( m_spFrame.number_of_particles );
				for ( unsigned int pn = 0; pn < m_spFrame.number_of_particles; ++pn )
				{
					SMayaParticle& p = m_spFrame.particles[pn];
					array[pn].pos = math::Vec3f(p.x, p.y, p.z);
					array[pn].size = math::Vec2f(p.scale, p.scale);
					array[pn].spin = p.spin * 3.1415926f/180.0f;

					math::Color c;
					c.set(p.r, p.g, p.b, p.a);
					array[pn].color = c.color;

					//if (!m_bIsAnimTextureUsed)
					//{
					//	agl::AglTexCoords uv1, uv2;
					//	uv1.tu = 0; uv1.tv = 0;
					//	uv2.tu = 1; uv2.tv = 1;
					//	(*uvs).v[0] = uv1;
					//	(*uvs).v[1] = uv2;
					//}
					//else
					//{
					//	agl::AglTexCoords uv1, uv2;

					//	unsigned char m_spFrame = p.tex_num;

					//	if (m_spFrame > m_ucTexFrames)
					//		m_spFrame = m_ucTexFrames;

					//	int x = (m_spFrame - 1) / m_ucCol;
					//	int y = m_spFrame - m_ucCol * x;
					//	x += 1;
					//	if (y == 0) y = 1;

					//	uv1.tu = (x - 1) * 1.0f / m_ucTrow;
					//	uv1.tv = (y - 1) * 1.0f / m_ucTcol;
					//						
					//	uv2.tu = x * 1.0f / m_ucTrow;
					//	uv2.tv = y * 1.0f / m_ucTcol;

					//	(*uvs).v[0] = uv1;
					//	(*uvs).v[1] = uv2;
					//}
				}
				spTank->update();
			}
			m_Frames = &psyst;
		}

		m_bIsSeqLoaded  = true;
	}
	//-----------------------------------------------------------------------------------
	void IMayaEmitter::setIntense(bool intense)
	{
		m_bIntense = intense;
	}
	//-----------------------------------------------------------------------------------
	void IMayaEmitter::setTexture(std::string tex)
	{
		if ( tex.length() != 0 )
		{
			m_TexName = tex;	
			loadTexture();
			//m_TexName.clear();
		}
	}
	//-----------------------------------------------------------------------------------
	void IMayaEmitter::render(unsigned int frame_num)
	{
		PTanks& Frames = *m_Frames;
		PPTank spTank = Frames[(unsigned int)frame_num];
		spTank->render(m_texture, m_Transform);
	}
	//----------------------------------------------------------------------------------------
	void IMayaEmitter::debugDraw()
	{
		m_Transform.debugDraw();

		unsigned frame_num = (int)(m_fLastFrame - m_time_shift);
		if (frame_num < 0) return;

		PFramesIter& it = ms_PFrames.find(m_Name);
		assert( it != ms_PFrames.end() );
		PTanks& psyst = it->second;
		assert( frame_num < psyst.size() );

		math::Matrix44f m = m_Transform.getFullTransform();

		render::Line3dManager& line_manager = render::TheLine3dManager::Get();

		PPTank spTank = psyst[frame_num];
		PTank::ParticleArray& array = spTank->getParticles();

		for ( PTank::ParticleArrayIter it = array.begin(); it != array.end(); ++it )
		{
			//if (!m_bIsGlobal)
			math::Vec3f center = m * (math::Point3f)it->pos;
			//else
			//	center = it->pos;

			line_manager.addQuad( center, it->size, it->spin );	
		}
	}
	//-----------------------------------------------------------------------------------
	void IMayaEmitter::fromStream(io::IReadStream& rf)
	{
		IEmitter::fromStream (rf);

		unsigned version;
		rf  >> version;
		if( version != ms_nVersion )
			assert(false);

		std::string seqName;
		rf	>> seqName
			>> m_TexName
			>> m_time_shift
			>> m_bIntense
			>> m_is_visible
			>> m_bCycling
			>> m_cTexFps;
		
		loadTexture();
		loadFrames(seqName);

		setIntense(m_bIntense);
		setCycling( m_bCycling );
		setVisible( m_is_visible );
	}
	//-----------------------------------------------------------------------------------
	void IMayaEmitter::toStream(io::IWriteStream& wf) const
	{
		IEmitter::toStream (wf);

		wf  << ms_nVersion
			<< m_Name
			<< m_TexName
			<< m_time_shift
			<< m_bIntense
			<< m_is_visible
			<< m_bCycling
			<< m_cTexFps;
	}

}