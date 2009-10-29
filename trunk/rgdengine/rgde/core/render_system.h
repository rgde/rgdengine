#pragma once

namespace core
{
	class render_system;
	typedef boost::shared_ptr<render_system> render_system_ptr;

	class application;

	class render_system : boost::noncopyable
	{
	public:		
		static render_system_ptr create(const application& app);

		virtual void save_screen(const std::wstring& file_name) = 0;
		virtual void update() const = 0;

	protected:
		virtual ~render_system() {}		
	};
}