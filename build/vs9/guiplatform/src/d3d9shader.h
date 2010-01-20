namespace gui
{
	namespace win32dx9platform
	{
		

		class D3D9ShaderEffect: public IShaderEffect
		{
			LPD3DXEFFECT m_pEffect;
			LPDIRECT3DDEVICE9 m_pDevice;
			bool Load(const std::wstring& name);
			void Unload();

		public:

			D3D9ShaderEffect(LPDIRECT3DDEVICE9 pDevice, const std::wstring& name);
		   ~D3D9ShaderEffect();

			bool SetTechnique(const std::string& tech_name);
		    unsigned Begin(unsigned int flags);
			bool BeginPass(unsigned int pass);
			bool EndPass();
			bool End();

			D3DXHANDLE GetParameterByName(D3DXHANDLE hParameter, LPCSTR pName);


			bool SetInt(D3DXHANDLE h, int value);
			bool SetBool(D3DXHANDLE h, bool value);
			bool SetFloat(D3DXHANDLE h, float value);
			bool SetTexture(D3DXHANDLE h, Texture* value);
		
			bool SetFloatArray(D3DXHANDLE h, const float* pf, unsigned int Count);
			void CommitChanges();

			void OnResetDevice();
			void OnLostDevice();
			
		};
	}
}