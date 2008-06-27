#pragma once

namespace core
{
	class IRenderSystem;
	typedef boost::shared_ptr<IRenderSystem> PRenderSystem;

	class application;

	class IRenderSystem : boost::noncopyable
	{
	public:		
		static PRenderSystem Create(const application& app);

		virtual void saveScreenShot(const std::wstring& file_name) = 0;
		virtual void update() const = 0;

	protected:
		virtual ~IRenderSystem() {}		
	};
}