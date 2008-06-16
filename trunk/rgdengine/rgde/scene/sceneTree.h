#pragma once

#include <rgde/core/xmlNode.h>
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

	class CNode : public core::XmlNode<CNode>
	{
	public:
		CNode(std::string name) : core::XmlNode<CNode>(name) {}
		virtual	~CNode(){m_children.clear();}

		virtual void update(double dTime, double dElapsedTime);
		virtual void clear();

		inline NodeType	getNodeType() const {return m_node_type;}

	protected:
		virtual void toStream(io::IWriteStream& wf);
		virtual void fromStream(io::IReadStream& rf);

	protected:
		NodeType	 m_node_type;
	};

	class NodeFolder : public CNode
	{
	public:
		NodeFolder(const std::string& name) 
			:	CNode(name)				
		{
			m_node_type = Folder;
		}
	};

	class CSceneTree : public io::ISerializedObject 
	{
	public:
		CSceneTree() :  m_root_node("Root") {}
		virtual				~CSceneTree(){}

		inline CNode&		 getRootNode(){return m_root_node;}

	protected:
		virtual void		 toStream(io::IWriteStream& wf);
		virtual void		 fromStream(io::IReadStream& rf);

	protected:
		CNode	m_root_node;
	};


	typedef  factory::TFactory<CNode> SceneFactory;

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