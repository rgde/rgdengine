#include "StdAfx.h"
#include "d3d9renderer.h"
#include "d3d9texture.h"
#include "d3d9shader.h"


gui::win32dx9platform::D3D9ShaderEffect::D3D9ShaderEffect( LPDIRECT3DDEVICE9 pDevice,const std::wstring& name ) :m_pDevice(pDevice)
{
	Load(name);
}

bool gui::win32dx9platform::D3D9ShaderEffect::Load( const std::wstring& name )
{

	return SUCCEEDED(D3DXCreateEffectFromFileW(m_pDevice, name.c_str(), NULL, NULL, 0, NULL, &m_pEffect, NULL));
}

bool gui::win32dx9platform::D3D9ShaderEffect::SetTechnique( const std::string& tech_name )
{
	return SUCCEEDED(m_pEffect->SetTechnique(tech_name.c_str()));
}


bool gui::win32dx9platform::D3D9ShaderEffect::BeginPass( unsigned int pass )
{
	return SUCCEEDED(m_pEffect->BeginPass(pass));
}

bool gui::win32dx9platform::D3D9ShaderEffect::EndPass()
{
	return SUCCEEDED(m_pEffect->EndPass());
}

bool gui::win32dx9platform::D3D9ShaderEffect::End()
{
	return SUCCEEDED(m_pEffect->End());
}



void gui::win32dx9platform::D3D9ShaderEffect::CommitChanges()
{
	m_pEffect->CommitChanges();
}

void gui::win32dx9platform::D3D9ShaderEffect::OnResetDevice()
{
	m_pEffect->OnResetDevice();
}

void gui::win32dx9platform::D3D9ShaderEffect::OnLostDevice()
{
	m_pEffect->OnLostDevice();
}

void gui::win32dx9platform::D3D9ShaderEffect::Unload()
{
	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = NULL;
	}
}
unsigned int gui::win32dx9platform::D3D9ShaderEffect::Begin(unsigned int flags)
{
	unsigned int i;
	m_pEffect->Begin(&i,flags);
	return i;
}

gui::win32dx9platform::D3D9ShaderEffect::~D3D9ShaderEffect()
{
	Unload();
}

D3DXHANDLE gui::win32dx9platform::D3D9ShaderEffect::GetParameterByName( D3DXHANDLE hParameter, LPCSTR pName )
{
	return m_pEffect->GetParameterByName(hParameter, pName);
}

bool gui::win32dx9platform::D3D9ShaderEffect::SetInt( D3DXHANDLE h, int value )
{
	return SUCCEEDED(m_pEffect->SetInt(h, value));
}

bool gui::win32dx9platform::D3D9ShaderEffect::SetBool( D3DXHANDLE h, bool value )
{
	return SUCCEEDED(m_pEffect->SetBool(h, value));
}

bool gui::win32dx9platform::D3D9ShaderEffect::SetFloat( D3DXHANDLE h, float value )
{
	return SUCCEEDED(m_pEffect->SetFloat(h, value));
}

bool gui::win32dx9platform::D3D9ShaderEffect::SetTexture( D3DXHANDLE h, Texture* value )
{
#ifdef DEBUG
	DirectX9Texture* d3dtexture = dynamic_cast <DirectX9Texture*> (value);
#else
	DirectX9Texture* d3dtexture = static_cast <DirectX9Texture*> (value);
#endif
	assert(d3dtexture);

	return SUCCEEDED(m_pEffect->SetTexture(h, d3dtexture->getD3DTexture()));
}
bool gui::win32dx9platform::D3D9ShaderEffect::SetFloatArray( D3DXHANDLE h, const float* pf, unsigned int Count )
{
	return SUCCEEDED(m_pEffect->SetFloatArray(h, pf, Count));
}