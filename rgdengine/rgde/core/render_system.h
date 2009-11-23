#pragma once

namespace render
{
	class render_device;
	typedef boost::shared_ptr<render_device> render_device_ptr;
}

namespace core
{
	class render_system;
	typedef boost::shared_ptr<render_system> render_system_ptr;

	class render_device_impl;	

	class application;	

	class render_system : boost::noncopyable
	{
		typedef boost::shared_ptr<render_device_impl> render_device_impl_ptr;
	public:		
		static render_system_ptr create(const application& app);
		static render_system* get();

		~render_system();

		virtual void save_screen(const std::wstring& file_name);
		virtual void update() const;

		virtual render::render_device& get_device();
		virtual const render::render_device& get_device() const;

	protected:
		render_system(const application& app, render_device_impl_ptr device);		

		render_device_impl_ptr m_render_device;
		const application& m_app;
	};
}