#pragma once

#include <rgde/core/xml_node.h>
#include <rgde/core/factory.h>
#include <rgde/io/io.h>

namespace scene
{
	class node : public core::meta_node<node>
	{
	public:
		enum type
		{
			Folder = 1,
			Entity,
			VisualEntity, 
			System, 
			TrasformFrame,
			Light,
			PartileEmitter
		};

		node(std::string name) : core::meta_node<node>(name) {}
		virtual	~node(){m_children.clear();}

		virtual void update(double time, double elapsed_time);
		virtual void clear();

		inline node::type	getNodeType() const {return m_node_type;}

	protected:
		virtual void to_stream(io::write_stream& wf);
		virtual void from_stream(io::read_stream& rf);

	protected:
		node::type	 m_node_type;
	};

	class folder_node : public node
	{
	public:
		folder_node(const std::string& name) 
			:	node(name)				
		{
			m_node_type = Folder;
		}
	};

	class scene_tree : public io::serialized_object 
	{
	public:
		scene_tree();
		virtual	~scene_tree();

		inline node&		 get_root(){return m_root_node;}

	protected:
		virtual void		 to_stream(io::write_stream& wf);
		virtual void		 from_stream(io::read_stream& rf);

	protected:
		node	m_root_node;
	};


	//typedef  core::factory<node> SceneFactory;

	//template <class T>
	//class _registrator
	//{
	//public:
	//	_registrator(){SceneFactory::RegisterCreator(new factory::TCreator<T>());}
	//protected:
	//	static _registrator<T> instance;
	//};

	//#define REGISTER_TYPE(type) _registrator<type> _registrator<type>::instance;
}