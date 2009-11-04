
namespace rgde
{
	class BatcherApplication : public core::windows::window
	{
	public:
		BatcherApplication(int x, int y, int w, int h, const std::wstring& title);
		~BatcherApplication();

		void update();
	private:
		render::device m_device;
	};
}