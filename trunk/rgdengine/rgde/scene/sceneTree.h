#pragma once

#include <rgde/core/xmlNode.h>
#include <rgde/core/factory.h>
#include <rgde/io/io.h>

// гавно !!!
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
	//public:
	//	typedef std::list<CNode*> ChildrenList;
	//	typedef std::list<CNode*>::iterator ChildIterator;
	public:
		CNode(std::string name) : core::XmlNode<CNode>(name) {}
		virtual				~CNode(){m_children.clear();}

		//inline CNode*		 getParent() {return m_parent;}
		//inline ChildrenList& getChildren(){return m_children;}
		//
		//inline void			 addChild(CNode* node){m_children.push_back(node);}
		//inline void			 removeChild(CNode* node){m_children.remove(node);}

		//inline CNode*		 getChild(std::string name){return 0;} ///todo
		//inline void			 removeChild(std::string name){} ///todo

		virtual void		 update(double dTime, double dElapsedTime);
		virtual void		 clear();

		inline NodeType		 getNodeType() const {return m_node_type;}

	protected:
		virtual void		 toStream(io::IWriteStream& wf);
		virtual void		 fromStream(io::IReadStream& rf);
	protected:
		NodeType	 m_node_type;
		
		//CNode*		 m_parent;
		//ChildrenList m_children;
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