#pragma once


namespace base
{
namespace composite
{
	namespace exceptions
	{
		struct invalid_component : public std::exception
		{
			invalid_component(const char* msg = "Invalid component") 
				: std::exception(msg) 
			{}
		};

		struct invalid_component_type : public invalid_component
		{
			invalid_component_type(const char* msg = "Invalid component type") 
				: invalid_component(msg) 
			{}
		};
	}

	class Component : public CBaseRefCounted, public core::CNamedObject
	{
	public:
		Component() : m_pParent(0) {}
		virtual ~Component() {}
		
		/// for basic type detection
		virtual bool isComposite() const {return false;}

		Component*  getParent() const {return m_pParent;}
		void		setPatent(Component* parent) 
		{
			onParentChange(parent); 
			m_pParent = parent;
		}

	protected:
		virtual void onParentChange(Component* new_parent) {}

	private:
		Component* m_pParent;
	};

	typedef boost::intrusive_ptr<Component> PComponent;


	//class Composite : public Component
	//{
	//public:
	//	typedef std::vector<PComponent> Components;
	//	typedef Components::iterator ComponentsIter;
	//	typedef Components::const_iterator ComponentsConstIter;

	//	virtual				~Composite() {}

	//	virtual void		isComposite() const {return true;}

	//	void				addComponent(PComponent component)
	//	{
	//	}

	//	void				removeComponent(PComponent component)
	//	{
	//	}

	//	size_t				getComponetsNum()		const	{return m_components;}
	//	PComponent			getComponent(size_t i)			{return m_components[i];}
	//	const PComponent&	getComponent(size_t i)	const	{return m_components[i];}

	//	const Components&	getComponents()			const	{return m_components;}

	//protected:
	//	virtual void		onAddComponent(PComponent component) 
	//	{
	//	}

	//	virtual void		onRemoveComponent(PComponent component) 
	//	{
	//	}


	//protected:
	//	Components m_components;
	//};

} // composite
} // base