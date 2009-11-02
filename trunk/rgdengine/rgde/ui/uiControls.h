#pragma once

namespace ui
{
	class base_control : public core::named_object/*, 
					 virtual public CPropertyOwner*/
	{
	public:
		typedef std::list<base_control*> children_list;

		virtual ~base_control(){}

		virtual void onEvent(const Event& e) = 0;

		virtual const math::Rect& getRect() = 0;
		virtual void setRect(const math::Rect&) = 0;

		/*virtual const children_list& getChildren() = 0;

		virtual void addChild(base_control*)  = 0;
		virtual void removeChild(base_control*)  = 0;		*/
	};

	class child_control : public virtual base_control
	{
	public:
		virtual const base_control* getParent()  = 0;
	};

	class base_button : public virtual child_control
	{
	public:
		virtual const std::wstring& get_text() = 0;
		virtual void set_text(const std::wstring&) = 0;
	};

	class base_label : public child_control
	{
	public:
		virtual const std::wstring& get_text() = 0;
		virtual void set_text(const std::wstring&) = 0;
	};

	class base_window : public virtual base_control
	{
	public:
		virtual const std::wstring& get_caption() = 0;
		virtual void set_caption(const std::wstring&) = 0;
	};
}