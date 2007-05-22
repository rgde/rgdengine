#pragma once 

#include <rgde/math/mathTransform.h>
#include <rgde/math/mathAnimationController.h>

#include <rgde/game/DynamicObject.h>

#include <rgde/render/renderMaterial.h>
#include <rgde/render/renderMesh.h>

namespace render 
{

	typedef boost::intrusive_ptr<class CModel> PModel;

	class CModel :  public math::CFrame, public game::IDynamicObject
	{
	public:
		typedef std::map<int, PMaterial>				MaterialMap;
		typedef std::vector<math::PFrame>				Frames;
		typedef std::vector<PMesh>						Meshes;
		typedef std::vector<math::CFrameAnimationController>	Controllers;

		static PModel Create(const std::string& strFileName);
		~CModel();
		
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
		CModel();

		void clear( void );
		void update( float dt );
		//Neonic: octree
		void updateTree( void );

		void load(const std::string& strFileName);

	protected:
		MaterialMap		m_vMaterials;
		Frames			m_vFrames;
		Meshes			m_vMeshes;
		Controllers		m_vControllers;

		bool			m_bVisible;
        
		std::string		m_strName;
		std::string		m_strComment;
	};	
}