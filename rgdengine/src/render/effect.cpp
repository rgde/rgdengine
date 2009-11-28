#include "precompiled.h"

#include <rgde/render/effect.h>
#include <rgde/render/render_device.h>

#include <rgde/core/application.h>

#include <d3dx9.h>
#include "texture_impl.h"

#include "../base/exception.h"

extern LPDIRECT3DDEVICE9 g_d3d;

namespace
{
	struct dx_include_impl : public ID3DXInclude
	{
		STDMETHOD(Open)(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)		
		{
			__asm nop;

			io::file_system& fs = io::file_system::get();
			// already set in effect::load
			//io::path_add_scoped p("Common/shaders/");
			io::readstream_ptr in = fs.find(pFileName);

			if (!in)
			{
				base::lerr << "include fx file not found: " << pFileName;

				*pBytes = 0;
				*ppData = 0;

				return S_FALSE;
			}

			uint size = in->get_size();
			byte* data = new byte[size];
			in->read(data, size);

			*pBytes = size;
			*ppData = data;

			return S_OK;
		}

		STDMETHOD(Close)(LPCVOID data)
		{
			byte* data_bytes = (byte*)data;
			delete []data_bytes;
			return S_OK;
		}
	} __include_impl;
}


namespace render
{
	void getAnnotation(ID3DXEffect* effect, D3DXHANDLE hParentHandle, int id, effect::annotation& annotation)
	{
		D3DXHANDLE hAnnotationHandle = effect->GetAnnotation(hParentHandle, id);
					
		D3DXPARAMETER_DESC desc;
		effect->GetParameterDesc(hAnnotationHandle, &desc);
		annotation.name = desc.Name;
		if(desc.Type != D3DXPT_STRING)
		{
			//base::lmsg << "annotation "<< annotation.name << " type isn't string. annotation has been skiped...";
			return;
		}
		LPCSTR strAnnotationValue;
		effect->GetString(hAnnotationHandle, &strAnnotationValue);
		annotation.value = strAnnotationValue;
		//base::lmsg << "annotation name: " << annotation.name << ", value: " << annotation.value;
	}

	//------------------------------------------------------------------------------
	namespace
	{
		struct EffectEntry
		{
			effect_ptr			effect;
			std::string		name;
		};

		typedef std::list<EffectEntry> EffectsList;
		EffectsList effects;

		struct _seacher
		{
			const std::string  &name;
			_seacher(const std::string &n) : name(n) {}

			template<class T>
			bool operator()(const T& bm) const
			{
				return name == bm.name;
			}
		};
	}

	//------------------------------------------------------------------------------
	// effect parameter implementation.
	//------------------------------------------------------------------------------
	class effect_param_impl: public effect::parameter
	{
	public:
		effect_param_impl(ID3DXEffect* effect, unsigned int index)
		{
			//guard(render::effect::effect_param_impl())
			m_effect = effect;
			D3DXHANDLE paramHandle = m_effect->GetParameter(NULL, index);

			if( NULL == paramHandle )
				throw std::exception("NULL parameter handler");
				//base::lerr << "EffectParam::EffectParam(): NULL parameter handler at position " << index;

			D3DXPARAMETER_DESC paramDesc;
			m_effect->GetParameterDesc(paramHandle, &paramDesc);

			m_type = (type)paramDesc.Type;
			m_name = paramDesc.Name;
			m_size = paramDesc.Bytes;

			if (NULL != paramDesc.Semantic)
				m_semantic = paramDesc.Semantic;
			else
				m_semantic = m_name;

			m_Handle = m_effect->GetParameterByName(NULL, m_name.c_str());

			//base::lmsg << "Annotations: " << paramDesc.Annotations;

			for(unsigned int i = 0; i < paramDesc.Annotations; i++)
			{
				effect::annotation annotation;
				getAnnotation(m_effect, m_Handle, i, annotation);
				m_vecAnnotations.push_back(annotation);
			}
			//unguard
		}

		effect::annotations_vector& get_annotations()
		{
			return m_vecAnnotations;
		}
		
		const std::string& get_name() const
		{
			return m_name;
		}

		const std::string& get_semantic() const
		{
			return m_semantic;
		}

		unsigned int get_size() const
		{
			return m_size;
		}

		type get_type() const
		{
			return m_type;
		}

		bool set(const void* data, unsigned int size)
		{
			if (FAILED(m_effect->SetValue(m_Handle, data, size)))
			{
				base::lwrn << "const void* data, unsigned int size.";
				return false;
			}

			return true;
		}

		bool set(int value)
		{
			if (FAILED(m_effect->SetInt(m_Handle, value)))
			{
				base::lwrn << "EffectParam::set(int value) failed.";
				return false;
			}

			return true;
		}

		bool set(bool value)
		{
			if (FAILED(m_effect->SetBool(m_Handle, value)))
			{
				base::lwrn << "EffectParam::set(bool value) failed.";
				return false;
			}

			return true;
		}

		bool set(float value)
		{
			if (FAILED(m_effect->SetFloat(m_Handle, value)))
			{
				base::lwrn << "EffectParam::set(float value) failed.";
				return false;
			}

			return true;
		}

		bool set(const std::string& value)
		{
			if (FAILED(m_effect->SetString(m_Handle, value.c_str())))
			{
				base::lwrn << "EffectParam::set(std::string value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Matrix33f& value)
		{
			if (FAILED(m_effect->SetMatrix(m_Handle, (const D3DXMATRIX*)&value)))
			{
				base::lwrn << "EffectParam::set(math::Matrix33f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::matrix44f& value)
		{
			if (FAILED(m_effect->SetMatrix(m_Handle, (const D3DXMATRIX*)&value)))
			{
				base::lwrn << "EffectParam::set(math::matrix44f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Color& value)
		{
			if (FAILED(m_effect->SetVector(m_Handle, (const D3DXVECTOR4*)&math::vec4f(value))))
			{
				base::lwrn << "EffectParam::set(math::Color& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::vec4f& value)
		{
			if (FAILED(m_effect->SetVector(m_Handle, (const D3DXVECTOR4*)&value)))
			{
				base::lwrn << "EffectParam::set(math::vec4f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::vec3f& value)
		{
			if (FAILED(m_effect->SetVector(m_Handle, (const D3DXVECTOR4*)&value)))
			{
				base::lwrn << "EffectParam::set(math::vec3f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::vec2f& value)
		{
			if (FAILED(m_effect->SetVector(m_Handle, (const D3DXVECTOR4*)&value)))
			{
				base::lwrn << "EffectParam::set(math::vec2f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const texture_ptr& texture)
		{
			if (texture)
			{
				texture_d3d9 *pTexImpl = static_cast<texture_d3d9*>(texture.get());
				//base::lmsg << "bind texture: " << pTexImpl->get_filename();
				IDirect3DTexture9* pDxTex = pTexImpl->get_dx_texture();
				if (FAILED(m_effect->SetTexture(m_Handle, pDxTex)))
				{
					base::lwrn << "EffectParam::set(texture_ptr texture) failed.";
					return false;
				}
			}
			else 
			{
				if (FAILED(m_effect->SetTexture(m_Handle, NULL)))
				{
					base::lwrn << "EffectParam::set(texture_ptr texture = NULL) failed.";
					return false;
				}
			}

			return true;
		}

		bool set(const int* value, int num)
		{
			if (FAILED(m_effect->SetIntArray(m_Handle, value, num)))
			{
				base::lwrn << "EffectParam::set(int* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const float* value, int num)
		{
			if (FAILED(m_effect->SetFloatArray(m_Handle, value, num)))
			{
				base::lwrn << "EffectParam::set(float* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Matrix33f* value, int num)
		{
			if (FAILED(m_effect->SetMatrixArray(m_Handle, (const D3DXMATRIX*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::Matrix33f* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::matrix44f* value, int num)
		{
			if (FAILED(m_effect->SetMatrixArray(m_Handle, (const D3DXMATRIX*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::matrix44f* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::vec4f* value, int num)
		{
			if (FAILED(m_effect->SetVectorArray(m_Handle, (const D3DXVECTOR4*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::vec4f* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::vec3f* value, int num)
		{
			if (FAILED(m_effect->SetVectorArray(m_Handle, (const D3DXVECTOR4*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::vec3f* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::vec2f* value, int num)
		{
			if (FAILED(m_effect->SetVectorArray(m_Handle, (const D3DXVECTOR4*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::vec2f* value, int num) failed.";
				return false;
			}

			return true;
		}

	private:
		ID3DXEffect* m_effect;
		std::string m_name;
		std::string m_semantic;
		unsigned int m_size;
		type m_type;
		D3DXHANDLE m_Handle;
		effect::annotations_vector m_vecAnnotations;
	};

	//------------------------------------------------------------------------------
	// effect technique implementation.
	//------------------------------------------------------------------------------
	class effect_technique_impl: public effect::technique
	{
	public:
		class pass_impl: public pass
		{
		public:
			pass_impl(ID3DXEffect* effect, const D3DXHANDLE& technique, unsigned int index)
			{
				//guard(effect_technique_impl::pass_impl())

				m_effect = effect;
				D3DXPASS_DESC passDesc;

				D3DXHANDLE pass = m_effect->GetPass(technique, index);
				if (FAILED(m_effect->GetPassDesc(pass, &passDesc)))
					base::lerr << "Unable to get pass description";

				m_nCurrentPass = index;
				m_name = passDesc.Name;

				//base::lmsg << "pass_impl name: " << m_name;

				//------------------------------------------------------------------------------
				// ATTENTION: Number of pass annotations is zero, when the pass name is exist !!
				//            GetPassDesc returns S_OK and valid handle.
				//------------------------------------------------------------------------------
				//base::lmsg << "Annotations: " << passDesc.Annotations;

				for (unsigned int i = 0; i < passDesc.Annotations; i ++)
				{
					effect::annotation annotation;
					getAnnotation(m_effect, pass, i, annotation);
					m_vecAnnotations.push_back(annotation);
				}

				//unguard
			}

			~pass_impl()
			{
			}

			void begin()
			{
				//guard(effect_technique_impl::pass_impl::begin())
					m_effect->BeginPass(m_nCurrentPass);
				//unguard
			}

			void end()
			{
				//guard(effect_technique_impl::pass_impl::end())
					m_effect->EndPass();
				//unguard
			}

			const std::string& get_name() const
			{
				return m_name;
			}

			effect::annotations_vector& get_annotations()
			{
				return m_vecAnnotations;
			}
		private:
			unsigned int m_nCurrentPass;
			std::string m_name;
			ID3DXEffect* m_effect;
			effect::annotations_vector m_vecAnnotations;
		};

		effect_technique_impl(ID3DXEffect* effect, unsigned int index)
		{
			//guard(effect_technique_impl())

			m_effect = effect;
			techniqueHandle = m_effect->GetTechnique(index);
			if( !techniqueHandle )
				base::lerr << "effect_technique_impl::effect_technique_impl(): NULL parameter handler at position " << index;

			m_effect->GetTechniqueDesc(techniqueHandle, &Desc);

			//base::lmsg << "Num passes in technique '" << Desc.Name << "': " << Desc.Passes;
			for (unsigned int i = 0; i < Desc.Passes; i ++)
			{
				pass* pass = new pass_impl(m_effect, techniqueHandle, i);
				m_arPasses.push_back(pass);
			}

			m_name = Desc.Name;
			m_nPasses = Desc.Passes;
			m_nAnnotations = Desc.Annotations;

			//base::lmsg << "Annotations: " << Desc.Annotations;

			for (unsigned int i = 0; i < Desc.Annotations; i ++)
			{
				effect::annotation annotation;
				getAnnotation(m_effect, techniqueHandle, i, annotation);
				m_vecAnnotations.push_back(annotation);
			}

			//unguard
		}

		~effect_technique_impl()
		{
			m_effect = NULL;
		}

		std::vector <pass*>& get_passes()
		{
			return m_arPasses;
		}

		const std::string& get_name() const
		{
			return m_name;
		}

		effect::annotations_vector& get_annotations()
		{
			return m_vecAnnotations;
		}

		void begin()
		{
			//guard(effect_technique_impl::begin())
			if(NULL != m_effect)
			{
				m_effect->SetTechnique(m_name.c_str());
				unsigned int numPasses = 0;
				m_effect->Begin(&numPasses, 0);
			}
			//unguard
		}

		void end()
		{
			//guard(effect_technique_impl::end())
			if(NULL != m_effect)
				m_effect->End();
			//unguard
		}

		D3DXHANDLE getHandle() const
		{
			return techniqueHandle;
		}
	private:
		ID3DXEffect* m_effect;
		std::string m_name;
		D3DXTECHNIQUE_DESC Desc;
		unsigned int m_nPasses;
		unsigned int m_nAnnotations;
		std::vector <pass*> m_arPasses;
		D3DXHANDLE techniqueHandle;
		effect::annotations_vector m_vecAnnotations;
	};

	//------------------------------------------------------------------------------
	// effect implementation.
	//------------------------------------------------------------------------------
	class CEffect : public effect, device_object
	{
	public:
		friend effect_ptr;

		CEffect()
		{
			m_effect = 0;
		}

		bool load(const std::string& effect_name)
		{
			base::lmsg << "Loading Shader effect: " << effect_name;// std::string((char*)pErrors->GetBufferPointer());

			//guard(CEffect::load(std::wstring effect_name))

			if(NULL == m_spPool)
			{
				D3DXCreateEffectPool(&m_spPool);
				if(NULL == m_spPool)
				{
					base::lerr<<"CEffect::load(std::wstring effect_name): Can't create effect pool";
					//core::application::get()->close();
					return false;
				}	
			}

			m_name = effect_name;

			ID3DXBuffer* pErrors;

			try{
				io::file_system& fs = io::file_system::get();
				io::path_add_scoped p("Common/shaders/");
				io::readstream_ptr in = fs.find(m_name);

				if (!in)
				{
					base::lerr << "fx file not found: " << effect_name;
					return false;
				}
				
				std::vector<byte> data;
				io::stream_to_vector(data, in);

				V(D3DXCreateEffect(g_d3d, (void*)&(data[0]), (uint)data.size() , NULL, &__include_impl, render_device::get().get_shader_flags(), 
					m_spPool, &m_effect, &pErrors));
				//V(D3DXCreateEffectFromFile( g_d3d, m_name.c_str() , NULL, NULL, device_dx9::get().get_shader_flags(), 
				//	m_spPool, &m_effect, &pErrors));
			}
			catch(...)
			{
				std::string strError = std::string((char*)pErrors->GetBufferPointer());
				base::lerr << std::string((char*)pErrors->GetBufferPointer());
				return false;
			}

			D3DXEFFECT_DESC desc;
			V(m_effect->GetDesc(&desc));

			// Retrieve parameters.
			for(unsigned int i = 0; i < desc.Parameters; i ++)
			{
				parameter* effectParam = new effect_param_impl(m_effect, i);
				m_mapParameters[effectParam->get_semantic()] = effectParam;
			}

			// Retrieve techniques.
			for(unsigned int i = 0; i < desc.Techniques; i ++)
			{
				effect_technique_impl* technique = new effect_technique_impl(m_effect, i);

				if(FAILED(m_effect->ValidateTechnique(technique->getHandle())))
					base::lerr << "ValidateTechnique fault. effect file: " << effect_name << " tech: " << technique->get_name();
				else
					m_listTechniques.push_back(technique);
			}

			m_sNumEffects++;

			return true;
			//unguard
		}

		ID3DXEffect* m_effect;

		~CEffect()
		{
			//guard(~CEffect())
				SAFE_RELEASE(m_effect);
				if(m_sNumEffects == 1)
					SAFE_RELEASE(m_spPool);
				m_sNumEffects--;
			//unguard
		}

		void commit_changes()
		{
			//guard(CEffect::commit_changes())
			if(NULL != m_effect)
				m_effect->CommitChanges();
			//unguard
		}

		void onLostDevice()
		{
			//guard(CEffect::onLostDevice())
			if(NULL != m_effect)
				m_effect->OnLostDevice();
			//unguard
		}

		void onResetDevice()
		{
			//guard(CEffect::onResetDevice())
			if(NULL != m_effect)
				m_effect->OnResetDevice();
			//unguard
		}

		const std::string& get_name() const
		{
			return m_name;
		}

		params_map& get_params()
		{
			return m_mapParameters;
		}

		techniques_list& get_technics()
		{
			return m_listTechniques;
		}

		technique* find_technique(const std::string& name)
		{
			//guard(technique* CEffect::getTechnique(std::string name))

			techniques_list::iterator technique = m_listTechniques.begin();
			while (technique != m_listTechniques.end())
			{
				if ((*technique)->get_name() == name)
					return *technique;

				++ technique;
			}
			return 0;

			//unguard
		}
	protected:
		static LPD3DXEFFECTPOOL m_spPool;
		static int m_sNumEffects;
		std::string				m_name;
		params_map m_mapParameters;
		techniques_list m_listTechniques;
	};

	LPD3DXEFFECTPOOL CEffect::m_spPool = NULL;
	int CEffect::m_sNumEffects = 0;

	//------------------------------------------------------------------------------
	// Static methods.
	//------------------------------------------------------------------------------
	effect_ptr effect::create(const std::string& file_name)
	{
		//guard(effect_ptr effect::create(std::wstring file_name))

		EffectsList::iterator it = std::find_if(effects.begin(), effects.end(), _seacher(file_name));

		if (it != effects.end())
			return it->effect;

		EffectEntry ee;
		ee.effect = effect_ptr(new CEffect());
		bool res = ee.effect->load(file_name);
		ee.name = file_name;

		ee.effect = res ? ee.effect : effect_ptr();

		if (!res)
		{
			base::lerr << "effect::create(std::string effect_name): Can't load effect:" << file_name;
		}

		effects.push_back(ee);

		return ee.effect;

		//unguard
	}

	void effect::clear_all()
	{
		//guard(void effect::clear_all())
			effects.clear();
		//unguard
	}
}