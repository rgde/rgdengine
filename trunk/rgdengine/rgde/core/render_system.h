#pragma once

namespace render
{
	class render_device;
}

namespace core
{
	class render_system;
	typedef boost::shared_ptr<render_system> render_system_ptr;

	class application;	

	class render_system : boost::noncopyable
	{
	public:		
		static render_system_ptr create(const application& app);
		static render_system* get();

		virtual void save_screen(const std::wstring& file_name) = 0;
		virtual void update() const = 0;

		virtual render::render_device& get_device() = 0;
		virtual const render::render_device& get_device() const = 0;		

	protected:
		virtual ~render_system();
	};
}