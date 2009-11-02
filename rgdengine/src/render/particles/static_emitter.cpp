#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/static_emitter.h>
#include <rgde/render/particles/tank.h>

using namespace particles::maya_structs;

namespace particles{

	std::map< std::string, maya_structs::animation> static_emitter::m_FrSeq;
	static_emitter::frames_map static_emitter::ms_PFrames;

	static std::wstring ms_base_folder;

	int em_num = 0;

	//-----------------------------------------------------------------------------------
	void static_emitter::ClearCachedData()
	{
		ms_PFrames.clear();
		m_FrSeq.clear();

		em_num = 0;			// zero emitters counter
	}
	//-----------------------------------------------------------------------------------
	static_emitter::static_emitter(const std::string& sequence_name, const std::string texture_name) 
		: m_fScale(1.0f)
		, emitter(emitter::Static)
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

		m_texture_name				= texture_name;

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
	static_emitter::~static_emitter()
	{
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::loadTexture()
	{
		m_texture = render::texture::create(m_texture_name);
		m_bIsTexLoaded = true;
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::update(float dt)
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
	void static_emitter::render()
	{
		if (!(m_is_visible && m_bIsSeqLoaded))
			return;

		int frame_to_render = (int)(m_fLastFrame - m_time_shift);
		if (frame_to_render < 0) return;

		render((unsigned int)frame_to_render);
	}

	//-----------------------------------------------------------------------------------
	void static_emitter::loadFrames(std::string file_name)
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
			animation& fseq = m_FrSeq[m_Name];
		
			{
				std::string str = "Media/" + m_Name;
				io::read_file_stream pfxFile(str);
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
				anim_frame& m_spFrame = fseq.frames[i];
				renderer_ptr spTank(new renderer);
				psyst[i] = spTank;

				renderer::ParticleArray& array = spTank->getParticles();

				array.resize( m_spFrame.number_of_particles );
				for ( unsigned int pn = 0; pn < m_spFrame.number_of_particles; ++pn )
				{
					static_particle& p = m_spFrame.particles[pn];
					array[pn].pos = math::Vec3f(p.x, p.y, p.z);
					array[pn].size = math::Vec2f(p.scale, p.scale);
					array[pn].spin = p.spin * 3.1415926f/180.0f;

					math::Color c;
					c.set(p.r, p.g, p.b, p.a);
					array[pn].color = c.color;
				}
				spTank->update();
			}
			m_Frames = &psyst;
		}

		m_bIsSeqLoaded  = true;
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::setIntense(bool intense)
	{
		m_bIntense = intense;
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::setTexture(std::string tex)
	{
		if ( tex.length() != 0 )
		{
			m_texture_name = tex;	
			loadTexture();
			//m_texture_name.clear();
		}
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::render(unsigned int frame_num)
	{
		PTanks& frames_vector = *m_Frames;
		renderer_ptr spTank = frames_vector[(unsigned int)frame_num];
		spTank->render(m_texture, m_Transform);
	}
	//----------------------------------------------------------------------------------------
	void static_emitter::debug_draw()
	{
		m_Transform.debug_draw();

		unsigned frame_num = (int)(m_fLastFrame - m_time_shift);
		if (frame_num < 0) return;

		PFramesIter& it = ms_PFrames.find(m_Name);
		assert( it != ms_PFrames.end() );
		PTanks& psyst = it->second;
		assert( frame_num < psyst.size() );

		math::Matrix44f m = m_Transform.getFullTransform();

		render::Line3dManager& line_manager = render::TheLine3dManager::get();

		renderer_ptr spTank = psyst[frame_num];
		renderer::ParticleArray& array = spTank->getParticles();

		for ( renderer::ParticleArrayIter it = array.begin(); it != array.end(); ++it )
		{
			//if (!m_bIsGlobal)
			math::Vec3f center = m * (math::Point3f)it->pos;
			//else
			//	center = it->pos;

			line_manager.addQuad( center, it->size, it->spin );	
		}
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::fromStream(io::read_stream& rf)
	{
		emitter::fromStream (rf);

		unsigned version;
		rf  >> version;
		if( version != file_version )
			assert(false);

		std::string seqName;
		rf	>> seqName
			>> m_texture_name
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
	void static_emitter::toStream(io::write_stream& wf) const
	{
		emitter::toStream (wf);

		wf  << file_version
			<< m_Name
			<< m_texture_name
			<< m_time_shift
			<< m_bIntense
			<< m_is_visible
			<< m_bCycling
			<< m_cTexFps;
	}
}