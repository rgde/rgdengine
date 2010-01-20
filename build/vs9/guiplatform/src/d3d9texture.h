#pragma once

struct IDirect3DTexture9;

namespace gui
{
namespace win32dx9platform
{

class DirectX9Renderer;

class DirectX9Texture : public Texture
{
public:	
	DirectX9Texture(DirectX9Renderer& owner, IDirect3DTexture9* tex);
	virtual ~DirectX9Texture();
	
	IDirect3DTexture9*	getD3DTexture(void) const {return m_d3dtexture;}

	virtual	void	onDeviceLost(void);
	virtual	void	onDeviceReset(void);

protected:

	void	freeD3DTexture(void);
	virtual void	calculateMetrics();

	IDirect3DTexture9*	m_d3dtexture;
	std::string			m_filename;
    std::string			m_resourceGroup;
};

}
}