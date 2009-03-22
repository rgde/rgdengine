#pragma once

namespace ui
{
	class IControl : public core::named_object/*, 
					 virtual public CPropertyOwner*/
	{
	public:
		typedef std::list<IControl*> Children;

		virtual ~IControl(){}

		virtual void onEvent(const Event& e) = 0;

		virtual const math::Rect& getRect() = 0;
		virtual void setRect(const math::Rect&) = 0;

		/*virtual const Children& getChildren() = 0;

		virtual void addChild(IControl*)  = 0;
		virtual void removeChild(IControl*)  = 0;		*/
	};

	class IChildControl : public virtual IControl
	{
	public:
		virtual const IControl* getParent()  = 0;
	};

	class IButton : public virtual IChildControl
	{
	public:
		virtual const std::wstring& getText() = 0;
		virtual void setText(const std::wstring&) = 0;
	};

	class ILabel : public IChildControl
	{
	public:
		virtual const std::wstring& getText() = 0;
		virtual void setText(const std::wstring&) = 0;
	};

	class IWindow : public virtual IControl
	{
	public:
		virtual const std::wstring& getCaption() = 0;
		virtual void setCaption(const std::wstring&) = 0;
	};
}