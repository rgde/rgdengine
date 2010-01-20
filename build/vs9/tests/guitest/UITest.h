#pragma once

#include "guilib.h"

struct IDirect3DDevice9;

class UITest
{
public:
	UITest();
	virtual ~UITest();

	virtual void createGUISystem(IDirect3DDevice9* dev);
	virtual void resetGUISystem();

	virtual void Update(float delta);
	virtual void Render();

	virtual bool isFinished();

	bool isInitialized() { return m_system != 0; }

	bool handleMouseMove(int x, int y);
	bool handleMouseWheel(int diff);
	bool handleMouseButton(gui::EventArgs::MouseButtons btn, gui::EventArgs::ButtonState state);
	virtual bool handleKeyboard(UINT_PTR key, gui::EventArgs::ButtonState state);
	bool handleChar(UINT_PTR ch);

	void handleViewportChange();

	void load(const std::string& xml);

	virtual HRESULT OnResetDevice();
	virtual void	OnLostDevice();

protected:
	void log(gui::LogLevel level, const std::string& message);
	void reinit();

protected:
	std::string m_filename;

	gui::Renderer* m_render;
	gui::System* m_system;

	unsigned int m_framecount;
	HANDLE m_hFile;
};
