#include "precompiled.h"

#include <rgde/render/model.h>
#include <rgde/render/light.h>

#include <rgde/base/xml_helpers.h>

using namespace std;
using math::Color;

namespace render
{
	model::model() 
		: m_visible(true)
	{
	}

	model::~model()
	{
		clear();
	}

	void read_node(TiXmlElement *elem, math::frame &root_frame, model &model);
	mesh::geometry_ptr read_geometry(const std::string& fNm);

	model_ptr model::create(const std::string& filename)
	{
		model_ptr m = new model();
		m->load(filename);
        m->looped(true);
		m->play();
		m->update(0.0001f);			
		return m;
	}

	void model::load(const std::string& model_name)
	{
		base::lmsg << "loading model: " << "\"" << model_name << "\"";

		io::file_system &fs	= io::file_system::get();

		io::path_add_scoped p	("Models/" + model_name + "/");
		io::readstream_ptr in	= fs.find(model_name + ".xml");

		if (!in)
		{
			std::string error	= "model::load: can't load file ";// + filename;
			base::lerr << "model::load: can't load file: " << model_name << ".xml";
			throw exception(error.c_str());
		}

		std::vector<byte> data;
		io::stream_to_vector(data, in);

		TiXmlDocument xml;//(std::string(model_name.begin(), model_name.end()));
		xml.Parse((const char*)&(data[0]));

		clear();

		TiXmlElement *mod_elem	= xml.RootElement();
		TiXmlElement *elem		= mod_elem->FirstChild("material_list")->ToElement();

		if (elem)
		{
			for (TiXmlNode* node = elem->IterateChildren(NULL);
					node != 0; node = elem->IterateChildren(node))
			{
				TiXmlElement *tx= node->ToElement();
				int m_id= 0; 
				tx->Attribute("id", &m_id);

				std::string str	= tx->Attribute("file");
				m_materials[m_id] = material::create(str);
			}
		}
		{
			TiXmlNode *elem = mod_elem->FirstChild("node");

			if (elem)
				read_node(elem->ToElement(), *this, *this);
		}

		//Neonic: octree. Здесь обновляем дерево для всей модели.
		update();
	}

	void read_node(TiXmlElement *elem, math::frame &root_frame, model &model)
	{
		if (elem->Attribute("name"))
		{
			root_frame.name(elem->Attribute("name"));
		}

		//////////////////////////////////////////////////////////////////////////
		// load transformation data if exist
		{
			// translation
			if (TiXmlElement * node = elem->FirstChildElement("translation"))
			{
				math::vec3f v;
				base::read(v, node);
				root_frame.position(v);
			}

			//rotation
			if (TiXmlElement * node = elem->FirstChildElement("rotation"))
			{
				math::vec3f v;
				base::read(v, node);

				using math::Math::deg2Rad;				
				math::EulerAngleXYZf ang(deg2Rad(v[0]), deg2Rad(v[1]), deg2Rad(v[2]));

				math::quatf q;
				math::set(q, ang);

				root_frame.rotation(q);
			}

			//scale
			if (TiXmlElement * node = elem->FirstChildElement("scale"))
			{
				math::vec3f v;
				base::read(v, node);
				root_frame.scale(v);
			}
		}
		//////////////////////////////////////////////////////////////////////////

		TiXmlElement *gm	= elem->FirstChildElement("animation");

		//Neonic: octree
		bool dynamic = 0;

		if (gm)
		{
			math::frame_animation control;
			control.load(elem);

			control.atach(&root_frame);
			model.get_controllers().push_back(control);

			//Neonic: octree
			dynamic=1;
		}

		TiXmlElement *mt= elem->FirstChildElement("material");

		int m_id		= -1;

		if (mt)
			mt->Attribute("id", &m_id); 		   

		gm = elem->FirstChildElement("geometry");

		if (gm)
		{
			mesh_ptr m(new mesh);
			std::string mesh_file = std::string(gm->Attribute("name")) + ".xml";

			m->load(mesh_file);

			if (m_id >= 0)
				m->get_materials().push_back(model.get_materials()[m_id]);

			model.get_meshes().push_back(m);

			root_frame.add(m.get());
		}

		//By PC
		TiXmlElement *lt= elem->FirstChildElement("light");

		if (lt)
		{
			std::string strType = "point";

			TiXmlElement *type = lt->FirstChildElement("type");
			if(type)
				if(type->Attribute("value"))
					strType = type->Attribute("value");

			if(strType == "point")
			{
				PointLight *pPointLight = new PointLight(root_frame.name());
				root_frame.add(pPointLight);

				math::Color color;

				base::read_light_color(color, lt, "ambient");
				pPointLight->setAmbient(color);

				base::read_light_color(color, lt, "diffuse");
				pPointLight->setDiffuse(color);

				base::read_light_color(color, lt, "spec");
				pPointLight->setSpecular(color);

				float range = 1.0f;

				TiXmlElement *att = lt->FirstChildElement("far_att");
				if(att)
					if(att->Attribute("end"))
						range = base::lexical_cast<float, std::string>(att->Attribute("end"));

				pPointLight->range(range);
				pPointLight->constant_attenuation(0.0f);
				pPointLight->linear_attenuation(1.0f);
				pPointLight->quadratic_attenuation(0.0f);

				std::string strEnabled = "1";

				TiXmlElement *enabled = lt->FirstChildElement("enabled");
				if(enabled)
					if(enabled->Attribute("value"))
						strEnabled = enabled->Attribute("value");

				pPointLight->setEnabled(base::lexical_cast<int, std::string>(strEnabled)>0);
			}
			else
				base::lwrn<<"\""<<strType<<"\" light type temporally unsupported.";
		}


		TiXmlNode *cd	= 0;
		while (cd = elem->IterateChildren("node", cd))
		{
			math::frame_ptr child = math::frame::create();
			read_node(cd->ToElement(), *(child.get()), model);
			root_frame.add(child);
			model.getFrames().push_back(child); // а это нафига??
		};
	}

	mesh::geometry_ptr read_geometry(std::string fNm)
	{
		return mesh::geometry_ptr();
	}

	unsigned int model::getFaceNum() const
	{
		unsigned int ret= 0;
		meshes_vector::const_iterator i;
		for (i = m_meshes.begin(); i != m_meshes.end(); ++i)
			ret += (*i)->getNumPrimitives();
		return ret;
	}

	unsigned int model::get_num_verts() const
	{
		unsigned int ret= 0;
		meshes_vector::const_iterator i;
		for (i = m_meshes.begin(); i != m_meshes.end(); ++i)
			ret += (*i)->get_num_verts();
		return ret;
	}

	void model::clear( void )
	{
		m_materials.clear();
		m_frames.clear();
		meshes_vector::const_iterator i;
		//for (meshes_vector::const_iterator i = m_meshes.begin(); i != m_meshes.end(); ++i)
		//	(*i)->eject();
		m_meshes.clear();
		m_controllers.clear();
	};

	void model::update(float dt)
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).update(dt);
	}

	//Neonic: octree
	void model::update( void )
	{
		meshes_vector::const_iterator i;
		for ( i = m_meshes.begin(); i != m_meshes.end(); ++i )
			( *i )->update();
	};

	void model::play()
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).start();
	}

	void model::stop()
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).stop();
	}

	void model::pause()
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).pause();
	}

	void model::looped(bool flag)
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).looped(flag);
	}

	bool model::visible() const
	{
		return m_visible;
	}

	void model::visible(bool bVisible)
	{
		if (bVisible == m_visible)
			return;

		m_visible = bVisible;

		size_t nNumMeshes	= m_meshes.size();

		for (size_t i = 0; i < nNumMeshes; i++)
			if (m_visible)
				m_meshes[i]->show();
			else
				m_meshes[i]->hide();
	}
}