#pragma once

namespace gui
{


class GUILIB_API IShaderEffect
{
	public:
		//virtual bool Load(const std::wstring& name) = 0;

		virtual ~IShaderEffect() = 0 {};
		virtual bool SetTechnique(const std::string& tech_name) = 0;
		virtual unsigned int Begin(unsigned int flags) = 0;
		virtual bool BeginPass(unsigned int pass) = 0;
		virtual bool EndPass() = 0;
		virtual bool End() = 0;

		virtual D3DXHANDLE GetParameterByName(D3DXHANDLE hParameter, LPCSTR pName) = 0;

		virtual bool SetInt(D3DXHANDLE h, int value) = 0;
		virtual bool SetBool(D3DXHANDLE h, bool value) = 0;
		virtual bool SetFloat(D3DXHANDLE h, float value) = 0;
		virtual bool SetTexture(D3DXHANDLE h, Texture* value) = 0;
		virtual bool SetFloatArray(D3DXHANDLE h, const float* pf, unsigned int Count) = 0;
		

		virtual void CommitChanges() = 0;

		virtual void OnResetDevice() = 0;
		virtual void OnLostDevice() = 0;


		//virtual void Unload() = 0;

};
}