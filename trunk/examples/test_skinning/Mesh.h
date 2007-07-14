/*

@author PC
*/
#pragma once

template <class TVertex>
class TMesh : public math::CFrame, public render::IRendererable
{
public:
	struct SIndexedSubMeshInfo		
	{
		unsigned int nMaterialIndex;
		unsigned int nBaseVertexIndex;
		unsigned int nMinIndex;
		unsigned int nNumVertices; 
		unsigned int nStartIndex;
		unsigned int nPrimitiveCount;
	};

	typedef render::TIndexedGeometry<TVertex, false> Geometry;
	typedef boost::shared_ptr<Geometry>					PGeometry;
	typedef std::vector<render::PMaterial>						MaterialList;
	typedef std::vector<SIndexedSubMeshInfo>			SubMeshes;

	TMesh()
		: IRendererable(10)
	{
		m_renderInfo.pFrame = this;
		m_renderInfo.pRenderFunc = boost::bind(&TMesh::render, this);
		m_renderInfo.bHaveVolumes = true;
	}

	virtual void load(const std::string& file_name)
	{
		m_geometry = PGeometry(new Geometry());

		io::CFileSystem &fs	= io::TheFileSystem::Get();
		io::ScopePathAdd p	("Meshes/");

		m_geometry->load(file_name);

		m_renderInfo.bbox = m_geometry->getBBox();
		m_renderInfo.bsphere = m_geometry->getBSphere();
	}


	PGeometry&	  getGeometry()				{return m_geometry;}
	MaterialList& getMaterials()			{return m_materials;}

	render::PrimitiveType getPrimitiveType() const	{return m_ePrimType;}

	unsigned int  getNumPrimitives() const	{return int(m_geometry->getIB().size()/3);}

	unsigned int  getVertexNum()const
	{
/*		unsigned int ret= 0;
		for (SubMeshes::const_iterator it = m_sub_meshes.begin(); it != m_sub_meshes.end(); ++it)
		{
			ret += (*it).nNumVertices;
		}
		if (m_sub_meshes.empty())
			ret = m_vertex_num;
		return ret;*///WTTF?!?

		m_geometry->getVB().size();
	}

	void setEffect(render::PEffect spShader)
	{
		m_renderInfo.spShader = spShader;
		MaterialList::iterator it;
		for (it = m_materials.begin(); it != m_materials.end(); ++it)
		{
			(*it)->setEffect(spShader);
		}
	}

protected:
	virtual const render::SRenderableInfo&	getRenderableInfo() const
	{
		if (m_materials.size() > 0)
			m_renderInfo.spMaterial = *m_materials.begin();
		else
			m_renderInfo.spMaterial = render::PMaterial();

		return m_renderInfo;
	}

	virtual void render()
	{
		//return;
		// надо вводить понятие SubSet у меша - набор верщин со своим материалом.
		size_t numMaterials	= m_materials.size();
		MaterialList &mlist	= m_materials;

		if (0 == numMaterials)
		{
			m_geometry->render(m_ePrimType, getNumPrimitives());
		}
		else if (1 == numMaterials)
		{
			m_geometry->render(m_ePrimType, getNumPrimitives());
		}
		else
		{
			for (SubMeshes::iterator it = m_sub_meshes.begin(); it != m_sub_meshes.end(); ++it)
			{
				SIndexedSubMeshInfo &minfo	= *it;
				render::PMaterial m					= mlist[minfo.nMaterialIndex];

				m_geometry->render(m_ePrimType, minfo.nBaseVertexIndex, minfo.nMinIndex, minfo.nNumVertices, minfo.nStartIndex, minfo.nPrimitiveCount);
			}
		}
	}

protected:
	MaterialList	m_materials;
	SubMeshes		m_sub_meshes;
	PGeometry		m_geometry;

	render::PrimitiveType	m_ePrimType;				
};

typedef TMesh<vertex::MeshVertex> CMesh;
typedef boost::intrusive_ptr<CMesh> PMesh;