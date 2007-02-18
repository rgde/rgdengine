#pragma once

namespace scene
{
	class ISceneVisitor;
}

namespace base
{
	class IVisitor
	{
	public:
		virtual ~IVisitor(){}
	};

	template<class T>
	class TVisitor : public virtual IVisitor
	{
	public:
		virtual bool visit(T* t) = 0;
	};	

	class IVisitorAsseptor
	{
	public:
		virtual void asseptVisitor(scene::ISceneVisitor* visitor) = 0;
	};
}