#pragma once 

#include <rgde/math/transform.h>
#include <rgde/math/animation_controller.h>

#include <rgde/game/dynamic_object.h>

#include <rgde/render/material.h>
#include <rgde/render/mesh.h>

namespace render 
{

	typedef boost::intrusive_ptr<class Model> model_ptr;

	class Model : public math::Frame, public game::dynamic_object
	{
	public:
		typedef std::map<int, PMaterial>				MaterialMap;
		typedef std::vector<math::frame_ptr>				Frames;
		typedef std::vector<mesh_ptr>						Meshes;
		typedef std::vector<math::FrameAnimationController>	Controllers;

		static model_ptr create(const std::string& file_name);
		~Model();
		
		inline MaterialMap&  getMaterials()		{return m_vMaterials;}
		inline Frames&		 getFrames()		{return m_vFrames;}
		inline Meshes&		 getMeshes()		{return m_vMeshes;}
		inline Controllers&  getControllers()	{return m_vControllers;}

		unsigned int getVertexNum()	const;
		unsigned int getFaceNum()	const;

		bool isVisible() const;
		void setVisible(bool bVisible);

		void play();
		void stop();
		void pause();
		void setLooped( bool );

	protected:
		Model();

		void clear( void );
		void update( float dt );
		//Neonic: octree
		void updateTree( void );

		void load(const std::string& file_name);

	protected:
		MaterialMap		m_vMaterials;
		Frames			m_vFrames;
		Meshes			m_vMeshes;
		Controllers		m_vControllers;

		bool			m_is_visible;
        
		std::string		m_name;
		std::string		m_strComment;
	};	
}