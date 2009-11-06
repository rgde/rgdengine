#include "precompiled.h"

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/static_emitter.h>
#include <rgde/render/particles/tank.h>

using namespace particles::maya_structs;

namespace particles{

	std::map< std::string, maya_structs::animation> static_emitter::ms_frame_seq;
	static_emitter::frames_map static_emitter::ms_PFrames;

	static std::wstring ms_base_folder;

	int em_num = 0;

	//-----------------------------------------------------------------------------------
	void static_emitter::clear_cached_data()
	{
		ms_PFrames.clear();
		ms_frame_seq.clear();

		em_num = 0;			// zero emitters counter
	}
	//-----------------------------------------------------------------------------------
	static_emitter::static_emitter(const std::string& sequence_name, const std::string texture_name) 
		: m_fScale(1.0f)
		, emitter(emitter::Static)
	{
		m_bIntense				= false;

		m_is_seq_loaded			= false;
		m_is_texture_loaded			= false;

		m_is_fading				= false;

		m_is_visible				= false;
		m_bCycling				= false;

		m_time_shift			= 0;

		m_fLastFrame			= 0;
		m_fLastTime				= -1.0f;

		m_is_anim_texture_used	= false;
		m_texture_fps				= 25;

		m_texture_name				= texture_name;

		loadTexture();
		loadFrames(sequence_name);	
	}
	//-----------------------------------------------------------------------------------
	static_emitter::static_emitter() 
		: emitter(emitter::Static) 
	{
	}
	//-----------------------------------------------------------------------------------
	static_emitter::~static_emitter()
	{
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::reset()
	{
		m_fLastFrame = 0;
		m_fLastTime = 0;
		m_is_visible = true;
		m_is_fading = false;
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::loadTexture()
	{
		m_texture = render::texture::create(m_texture_name);
		m_is_texture_loaded = true;
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::update(float dt)
	{
		if (!m_is_seq_loaded) return;

		m_fLastFrame += dt * m_texture_fps;//25.0f;

		if (!(m_fLastFrame < m_time_shift + m_rames->size()))
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
		if (!(m_is_visible && m_is_seq_loaded))
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
			if (!m_is_texture_loaded){
				m_Name.clear();
				return;
			}

			bool just_reload = false;

			// повторно грузить то что уже было не надо :)
			if (m_Name == file_name) 
			{
				if (m_is_seq_loaded)
					return;
				else
					just_reload = true;
			}

			frames_iter it = ms_PFrames.find(file_name);
			m_Name = file_name;
		
			if (it != ms_PFrames.end())
				if (!just_reload)
				{
					m_is_seq_loaded = true;
					m_rames = &(ms_PFrames[m_Name]);
					return;
				}
				else 
					ms_PFrames.erase(file_name);
		}

		{
			animation& fseq = ms_frame_seq[m_Name];
		
			{
				std::string str = "Media/" + m_Name;
				io::read_file pfxFile(str);
				if(pfxFile.is_open())
				{
					if (!fseq.Load(pfxFile))
					{
						m_is_seq_loaded = false;
						return;
					}
				}
				else
				{
					//assert(false);
					return;
				}
			}

			renderers& psyst = ms_PFrames[m_Name];
			psyst.resize(fseq.frames.size());
			

			for (unsigned int i = 0; i < fseq.frames.size(); ++i)
			{
				anim_frame& m_spFrame = fseq.frames[i];
				renderer_ptr spTank(new renderer);
				psyst[i] = spTank;

				renderer::particles_vector& array = spTank->getParticles();

				array.resize( m_spFrame.number_of_particles );
				for ( unsigned int pn = 0; pn < m_spFrame.number_of_particles; ++pn )
				{
					static_particle& p = m_spFrame.particles[pn];
					array[pn].pos = math::vec3f(p.x, p.y, p.z);
					array[pn].size = math::vec2f(p.scale, p.scale);
					array[pn].spin = p.spin * 3.1415926f/180.0f;

					math::Color c;
					c.set(p.r, p.g, p.b, p.a);
					array[pn].color = c.color;
				}
				spTank->update();
			}
			m_rames = &psyst;
		}

		m_is_seq_loaded  = true;
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::setIntense(bool intense)
	{
		m_bIntense = intense;
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::set_texture(std::string tex)
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
		renderers& frames_vector = *m_rames;
		renderer_ptr spTank = frames_vector[(unsigned int)frame_num];
		spTank->render(m_texture, m_transform);
	}
	//----------------------------------------------------------------------------------------
	void static_emitter::debug_draw()
	{
		if (ms_PFrames.empty())
			return;

		m_transform->debug_draw();

		unsigned frame_num = (int)(m_fLastFrame - m_time_shift);
		if (frame_num < 0) return;

		frames_iter& it = ms_PFrames.find(m_Name);
		assert( it != ms_PFrames.end() );
		renderers& psyst = it->second;
		assert( frame_num < psyst.size() );

		const math::matrix44f& m = m_transform->get_full_tm();

		render::lines3d& line_manager = render::render_device::get().get_lines3d();

		renderer_ptr spTank = psyst[frame_num];
		renderer::particles_vector& array = spTank->getParticles();

		for ( renderer::ParticleArrayIter it = array.begin(); it != array.end(); ++it )
		{
			//if (!m_is_global)
			math::vec3f center = m * (math::point3f)it->pos;
			//else
			//	center = it->pos;

			line_manager.add_quad( center, it->size, it->spin );	
		}
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::from_stream(io::read_stream& rf)
	{
		emitter::from_stream (rf);

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
			>> m_texture_fps;
		
		loadTexture();
		loadFrames(seqName);

		setIntense(m_bIntense);
		setCycling( m_bCycling );
		setVisible( m_is_visible );
	}
	//-----------------------------------------------------------------------------------
	void static_emitter::to_stream(io::write_stream& wf) const
	{
		emitter::to_stream (wf);

		wf  << file_version
			<< m_Name
			<< m_texture_name
			<< m_time_shift
			<< m_bIntense
			<< m_is_visible
			<< m_bCycling
			<< m_texture_fps;
	}
}