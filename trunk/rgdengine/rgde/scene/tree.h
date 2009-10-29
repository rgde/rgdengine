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

		virtual void update(double dTime, double dElapsedTime);
		virtual void clear();

		inline node::type	getNodeType() const {return m_node_type;}

	protected:
		virtual void toStream(io::write_stream& wf);
		virtual void fromStream(io::read_stream& rf);

	protected:
		node::type	 m_node_type;
	};

	class NodeFolder : public node
	{
	public:
		NodeFolder(const std::string& name) 
			:	node(name)				
		{
			m_node_type = Folder;
		}
	};

	class SceneTree : public io::serialized_object 
	{
	public:
		SceneTree() :  m_root_node("Root") {}
		virtual				~SceneTree(){}

		inline node&		 getRootNode(){return m_root_node;}

	protected:
		virtual void		 toStream(io::write_stream& wf);
		virtual void		 fromStream(io::read_stream& rf);

	protected:
		node	m_root_node;
	};


	typedef  core::factory<node> SceneFactory;

	template <class T>
	class _registrator
	{
	public:
		_registrator(){SceneFactory::RegisterCreator(new factory::TCreator<T>());}
	protected:
		static _registrator<T> instance;
	};

	#define REGISTER_TYPE(type) _registrator<type> _registrator<type>::instance;
}