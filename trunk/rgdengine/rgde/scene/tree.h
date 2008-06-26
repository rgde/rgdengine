#pragma once

#include <rgde/core/xml_node.h>
#include <rgde/core/factory.h>
#include <rgde/io/io.h>

namespace scene
{
	enum NodeType
	{
		Folder = 1,
		Entity,
		VisualEntity, 
		System, 
		TrasformFrame,
		Light,
		PartileEmitter
	};

	class Node : public core::XmlNode<Node>
	{
	public:
		Node(std::string name) : core::XmlNode<Node>(name) {}
		virtual	~Node(){m_children.clear();}

		virtual void update(double dTime, double dElapsedTime);
		virtual void clear();

		inline NodeType	getNodeType() const {return m_node_type;}

	protected:
		virtual void toStream(io::IWriteStream& wf);
		virtual void fromStream(io::IReadStream& rf);

	protected:
		NodeType	 m_node_type;
	};

	class NodeFolder : public Node
	{
	public:
		NodeFolder(const std::string& name) 
			:	Node(name)				
		{
			m_node_type = Folder;
		}
	};

	class SceneTree : public io::ISerializedObject 
	{
	public:
		SceneTree() :  m_root_node("Root") {}
		virtual				~SceneTree(){}

		inline Node&		 getRootNode(){return m_root_node;}

	protected:
		virtual void		 toStream(io::IWriteStream& wf);
		virtual void		 fromStream(io::IReadStream& rf);

	protected:
		Node	m_root_node;
	};


	typedef  factory::TFactory<Node> SceneFactory;

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