#pragma once 

#include <rgde/math/transform.h>
#include <rgde/math/animation_controller.h>

#include <rgde/game/dynamic_object.h>

#include <rgde/render/material.h>
#include <rgde/render/mesh.h>

namespace render 
{

	typedef boost::intrusive_ptr<class model> model_ptr;

	class model : public math::frame, public game::dynamic_object
	{
	public:
		typedef std::map<int, material_ptr>					material_map;
		typedef std::vector<math::frame_ptr>				frames_vector;
		typedef std::vector<mesh_ptr>						meshes_vector;
		typedef std::vector<math::frame_anim_controller>	contollers_vector;

		static model_ptr create(const std::string& file_name);
		~model();
		
		inline material_map&  get_materials()		{return m_materials;}
		inline frames_vector&		 getFrames()		{return m_frames;}
		inline meshes_vector&		 get_meshes()		{return m_meshes;}
		inline contollers_vector&  get_controllers()	{return m_controllers;}

		unsigned int get_num_verts()	const;
		unsigned int getFaceNum()	const;

		bool is_visible() const;
		void setVisible(bool bVisible);

		void play();
		void stop();
		void pause();
		void set_looped( bool );

	protected:
		model();

		void clear( void );
		void update( float dt );
		//Neonic: octree
		void update( void );

		void load(const std::string& file_name);

	protected:
		material_map		m_materials;
		frames_vector		m_frames;
		meshes_vector		m_meshes;
		contollers_vector	m_controllers;

		bool			m_is_visible;
        
		std::string		m_name;
		std::string		m_strComment;
	};	
}