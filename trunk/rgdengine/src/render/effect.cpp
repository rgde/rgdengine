#include "precompiled.h"

#include <rgde/render/effect.h>
#include <rgde/render/device.h>

#include <rgde/core/application.h>

#include <d3dx9.h>
#include "texture_impl.h"

#include "../base/exception.h"

extern LPDIRECT3DDEVICE9 g_pd3dDevice;

namespace render
{
	void getAnnotation(ID3DXEffect* pEffect, D3DXHANDLE hParentHandle, int id, Effect::Annotation& annotation)
	{
		D3DXHANDLE hAnnotationHandle = pEffect->GetAnnotation(hParentHandle, id);
					
		D3DXPARAMETER_DESC desc;
		pEffect->GetParameterDesc(hAnnotationHandle, &desc);
		annotation.name = desc.Name;
		if(desc.Type != D3DXPT_STRING)
		{
			//base::lmsg << "Annotation "<< annotation.name << " type isn't string. Annotation has been skiped...";
			return;
		}
		LPCSTR strAnnotationValue;
		pEffect->GetString(hAnnotationHandle, &strAnnotationValue);
		annotation.value = strAnnotationValue;
		//base::lmsg << "Annotation name: " << annotation.name << ", value: " << annotation.value;
	}

	//------------------------------------------------------------------------------
	namespace
	{
		struct EffectEntry
		{
			PEffect			effect;
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
	// Effect parameter implementation.
	//------------------------------------------------------------------------------
	class Parameter: public Effect::IParameter
	{
	public:
		Parameter(ID3DXEffect* effect, unsigned int index)
		{
			//guard(render::Effect::Parameter())
			m_pEffect = effect;
			D3DXHANDLE paramHandle = m_pEffect->GetParameter(NULL, index);

			if( NULL == paramHandle )
				throw std::exception("NULL parameter handler");
				//base::lerr << "EffectParam::EffectParam(): NULL parameter handler at position " << index;

			D3DXPARAMETER_DESC paramDesc;
			m_pEffect->GetParameterDesc(paramHandle, &paramDesc);

			m_eType = (EType)paramDesc.Type;
			m_name = paramDesc.Name;
			m_nSize = paramDesc.Bytes;

			if (NULL != paramDesc.Semantic)
				m_strSemantic = paramDesc.Semantic;
			else
				m_strSemantic = m_name;

			m_Handle = m_pEffect->GetParameterByName(NULL, m_name.c_str());

			//base::lmsg << "Annotations: " << paramDesc.Annotations;

			for(unsigned int i = 0; i < paramDesc.Annotations; i++)
			{
				Effect::Annotation annotation;
				getAnnotation(m_pEffect, m_Handle, i, annotation);
				m_vecAnnotations.push_back(annotation);
			}
			//unguard
		}

		Effect::AnnotationsVector& getAnnotations()
		{
			return m_vecAnnotations;
		}
		
		const std::string& getName() const
		{
			return m_name;
		}

		const std::string& getSemantic() const
		{
			return m_strSemantic;
		}

		unsigned int getSize() const
		{
			return m_nSize;
		}

		EType getType() const
		{
			return m_eType;
		}

		bool set(const void* pData, unsigned int iSize)
		{
			if (FAILED(m_pEffect->SetValue(m_Handle, pData, iSize)))
			{
				base::lwrn << "const void* pData, unsigned int iSize.";
				return false;
			}

			return true;
		}

		bool set(int value)
		{
			if (FAILED(m_pEffect->SetInt(m_Handle, value)))
			{
				base::lwrn << "EffectParam::set(int value) failed.";
				return false;
			}

			return true;
		}

		bool set(bool value)
		{
			if (FAILED(m_pEffect->SetBool(m_Handle, value)))
			{
				base::lwrn << "EffectParam::set(bool value) failed.";
				return false;
			}

			return true;
		}

		bool set(float value)
		{
			if (FAILED(m_pEffect->SetFloat(m_Handle, value)))
			{
				base::lwrn << "EffectParam::set(float value) failed.";
				return false;
			}

			return true;
		}

		bool set(const std::string& value)
		{
			if (FAILED(m_pEffect->SetString(m_Handle, value.c_str())))
			{
				base::lwrn << "EffectParam::set(std::string value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Matrix33f& value)
		{
			if (FAILED(m_pEffect->SetMatrix(m_Handle, (const D3DXMATRIX*)&value)))
			{
				base::lwrn << "EffectParam::set(math::Matrix33f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Matrix44f& value)
		{
			if (FAILED(m_pEffect->SetMatrix(m_Handle, (const D3DXMATRIX*)&value)))
			{
				base::lwrn << "EffectParam::set(math::Matrix44f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Color& value)
		{
			if (FAILED(m_pEffect->SetVector(m_Handle, (const D3DXVECTOR4*)&math::Vec4f(value))))
			{
				base::lwrn << "EffectParam::set(math::Color& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Vec4f& value)
		{
			if (FAILED(m_pEffect->SetVector(m_Handle, (const D3DXVECTOR4*)&value)))
			{
				base::lwrn << "EffectParam::set(math::Vec4f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Vec3f& value)
		{
			if (FAILED(m_pEffect->SetVector(m_Handle, (const D3DXVECTOR4*)&value)))
			{
				base::lwrn << "EffectParam::set(math::Vec3f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Vec2f& value)
		{
			if (FAILED(m_pEffect->SetVector(m_Handle, (const D3DXVECTOR4*)&value)))
			{
				base::lwrn << "EffectParam::set(math::Vec2f& value) failed.";
				return false;
			}

			return true;
		}

		bool set(const PTexture& texture)
		{
			ITexture* pTex = NULL;
			if (texture) pTex = texture.get();

			if (pTex != NULL)
			{
				TextureImpl *pTexImpl = static_cast<TextureImpl*>(pTex);
				//base::lmsg << "bind texture: " << pTexImpl->getFileName();
				IDirect3DTexture9* pDxTex = pTexImpl->getDxTexture();
				if (FAILED(m_pEffect->SetTexture(m_Handle, pDxTex)))
				{
					base::lwrn << "EffectParam::set(PTexture texture) failed.";
					return false;
				}
			}
			else 
			{
				if (FAILED(m_pEffect->SetTexture(m_Handle, NULL)))
				{
					base::lwrn << "EffectParam::set(PTexture texture = NULL) failed.";
					return false;
				}
			}

			return true;
		}

		bool set(const int* value, int num)
		{
			if (FAILED(m_pEffect->SetIntArray(m_Handle, value, num)))
			{
				base::lwrn << "EffectParam::set(int* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const float* value, int num)
		{
			if (FAILED(m_pEffect->SetFloatArray(m_Handle, value, num)))
			{
				base::lwrn << "EffectParam::set(float* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Matrix33f* value, int num)
		{
			if (FAILED(m_pEffect->SetMatrixArray(m_Handle, (const D3DXMATRIX*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::Matrix33f* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Matrix44f* value, int num)
		{
			if (FAILED(m_pEffect->SetMatrixArray(m_Handle, (const D3DXMATRIX*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::Matrix44f* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Vec4f* value, int num)
		{
			if (FAILED(m_pEffect->SetVectorArray(m_Handle, (const D3DXVECTOR4*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::Vec4f* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Vec3f* value, int num)
		{
			if (FAILED(m_pEffect->SetVectorArray(m_Handle, (const D3DXVECTOR4*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::Vec3f* value, int num) failed.";
				return false;
			}

			return true;
		}

		bool set(const math::Vec2f* value, int num)
		{
			if (FAILED(m_pEffect->SetVectorArray(m_Handle, (const D3DXVECTOR4*)value, num)))
			{
				base::lwrn << "EffectParam::set(math::Vec2f* value, int num) failed.";
				return false;
			}

			return true;
		}

	private:
		ID3DXEffect* m_pEffect;
		std::string m_name;
		std::string m_strSemantic;
		unsigned int m_nSize;
		EType m_eType;
		D3DXHANDLE m_Handle;
		Effect::AnnotationsVector m_vecAnnotations;
	};

	//------------------------------------------------------------------------------
	// Effect technique implementation.
	//------------------------------------------------------------------------------
	class Technique: public Effect::ITechnique
	{
	public:
		class Pass: public IPass
		{
		public:
			Pass(ID3DXEffect* effect, const D3DXHANDLE& technique, unsigned int index)
			{
				//guard(Technique::Pass())

				m_pEffect = effect;
				D3DXPASS_DESC passDesc;

				D3DXHANDLE pass = m_pEffect->GetPass(technique, index);
				if (FAILED(m_pEffect->GetPassDesc(pass, &passDesc)))
					base::lerr << "Unable to get pass description";

				m_nCurrentPass = index;
				m_name = passDesc.Name;

				//base::lmsg << "Pass name: " << m_name;

				//------------------------------------------------------------------------------
				// ATTENTION: Number of pass annotations is zero, when the pass name is exist !!
				//            GetPassDesc returns S_OK and valid handle.
				//------------------------------------------------------------------------------
				//base::lmsg << "Annotations: " << passDesc.Annotations;

				for (unsigned int i = 0; i < passDesc.Annotations; i ++)
				{
					Effect::Annotation annotation;
					getAnnotation(m_pEffect, pass, i, annotation);
					m_vecAnnotations.push_back(annotation);
				}

				//unguard
			}

			~Pass()
			{
			}

			void begin()
			{
				//guard(Technique::Pass::begin())
					m_pEffect->BeginPass(m_nCurrentPass);
				//unguard
			}

			void end()
			{
				//guard(Technique::Pass::end())
					m_pEffect->EndPass();
				//unguard
			}

			const std::string& getName() const
			{
				return m_name;
			}

			Effect::AnnotationsVector& getAnnotations()
			{
				return m_vecAnnotations;
			}
		private:
			unsigned int m_nCurrentPass;
			std::string m_name;
			ID3DXEffect* m_pEffect;
			Effect::AnnotationsVector m_vecAnnotations;
		};

		Technique(ID3DXEffect* effect, unsigned int index)
		{
			//guard(Technique())

			m_pEffect = effect;
			techniqueHandle = m_pEffect->GetTechnique(index);
			if( !techniqueHandle )
				base::lerr << "Technique::Technique(): NULL parameter handler at position " << index;

			m_pEffect->GetTechniqueDesc(techniqueHandle, &Desc);

			//base::lmsg << "Num passes in technique '" << Desc.Name << "': " << Desc.Passes;
			for (unsigned int i = 0; i < Desc.Passes; i ++)
			{
				IPass* pass = new Pass(m_pEffect, techniqueHandle, i);
				m_arPasses.push_back(pass);
			}

			m_name = Desc.Name;
			m_nPasses = Desc.Passes;
			m_nAnnotations = Desc.Annotations;

			//base::lmsg << "Annotations: " << Desc.Annotations;

			for (unsigned int i = 0; i < Desc.Annotations; i ++)
			{
				Effect::Annotation annotation;
				getAnnotation(m_pEffect, techniqueHandle, i, annotation);
				m_vecAnnotations.push_back(annotation);
			}

			//unguard
		}

		~Technique()
		{
			m_pEffect = NULL;
		}

		std::vector <IPass*>& getPasses()
		{
			return m_arPasses;
		}

		const std::string& getName() const
		{
			return m_name;
		}

		Effect::AnnotationsVector& getAnnotations()
		{
			return m_vecAnnotations;
		}

		void begin()
		{
			//guard(Technique::begin())
			if(NULL != m_pEffect)
			{
				m_pEffect->SetTechnique(m_name.c_str());
				unsigned int numPasses = 0;
				m_pEffect->Begin(&numPasses, 0);
			}
			//unguard
		}

		void end()
		{
			//guard(Technique::end())
			if(NULL != m_pEffect)
				m_pEffect->End();
			//unguard
		}

		D3DXHANDLE getHandle() const
		{
			return techniqueHandle;
		}
	private:
		ID3DXEffect* m_pEffect;
		std::string m_name;
		D3DXTECHNIQUE_DESC Desc;
		unsigned int m_nPasses;
		unsigned int m_nAnnotations;
		std::vector <IPass*> m_arPasses;
		D3DXHANDLE techniqueHandle;
		Effect::AnnotationsVector m_vecAnnotations;
	};

	//------------------------------------------------------------------------------
	// Effect implementation.
	//------------------------------------------------------------------------------
	class CEffect : public Effect, IDeviceObject
	{
	public:
		friend PEffect;

		CEffect()
		{
			m_pEffect = 0;
		}

		void load(const std::string& effect_name)
		{
			//guard(CEffect::load(std::wstring effect_name))

			if(NULL == m_spPool)
			{
				D3DXCreateEffectPool(&m_spPool);
				if(NULL == m_spPool)
				{
					base::lerr<<"CEffect::load(std::wstring effect_name): Can't create Effect pool";
					core::application::Get()->close();
				}	
			}

			m_name = effect_name;

			ID3DXBuffer* pErrors;

			try{
				io::CFileSystem& fs = io::TheFileSystem::Get();
				io::ScopePathAdd p("Common/shaders/");
				io::PReadStream in = fs.findFile(m_name);

				if (!in)
				{
					base::lerr << "fx file not found: " << effect_name;
					return;
				}
				
				std::vector<byte> data;
				io::StreamToVector(data, in);

				V(D3DXCreateEffect(g_pd3dDevice, (void*)&(data[0]), (uint)data.size() , NULL, NULL, TheDevice::Get().getShaderFlags(), 
					m_spPool, &m_pEffect, &pErrors));
				//V(D3DXCreateEffectFromFile( g_pd3dDevice, m_name.c_str() , NULL, NULL, Device::Get().getShaderFlags(), 
				//	m_spPool, &m_pEffect, &pErrors));
			}
			catch(...)
			{
				std::string strError = std::string((char*)pErrors->GetBufferPointer());
				base::lerr << std::string((char*)pErrors->GetBufferPointer());
				return;
			}

			D3DXEFFECT_DESC desc;
			V(m_pEffect->GetDesc(&desc));

			// Retrieve parameters.
			for(unsigned int i = 0; i < desc.Parameters; i ++)
			{
				IParameter* effectParam = new Parameter(m_pEffect, i);
				m_mapParameters[effectParam->getSemantic()] = effectParam;
			}

			// Retrieve techniques.
			for(unsigned int i = 0; i < desc.Techniques; i ++)
			{
				Technique* technique = new Technique(m_pEffect, i);

				if(FAILED(m_pEffect->ValidateTechnique(technique->getHandle())))
					base::lerr << "ValidateTechnique fault. effect file: " << effect_name << " tech: " << technique->getName();
				else
					m_listTechniques.push_back(technique);
			}

			m_sNumEffects++;

			//unguard
		}

		ID3DXEffect* m_pEffect;

		~CEffect()
		{
			//guard(~CEffect())
				SAFE_RELEASE(m_pEffect);
				if(m_sNumEffects == 1)
					SAFE_RELEASE(m_spPool);
				m_sNumEffects--;
			//unguard
		}

		void commitChanges()
		{
			//guard(CEffect::commitChanges())
			if(NULL != m_pEffect)
				m_pEffect->CommitChanges();
			//unguard
		}

		void onLostDevice()
		{
			//guard(CEffect::onLostDevice())
			if(NULL != m_pEffect)
				m_pEffect->OnLostDevice();
			//unguard
		}

		void onResetDevice()
		{
			//guard(CEffect::onResetDevice())
			if(NULL != m_pEffect)
				m_pEffect->OnResetDevice();
			//unguard
		}

		const std::string& getName() const
		{
			return m_name;
		}

		Parameters& getParams()
		{
			return m_mapParameters;
		}

		Techniques& getTechnics()
		{
			return m_listTechniques;
		}

		ITechnique* findTechnique(const std::string& name)
		{
			//guard(ITechnique* CEffect::getTechnique(std::string name))

			Techniques::iterator technique = m_listTechniques.begin();
			while (technique != m_listTechniques.end())
			{
				if ((*technique)->getName() == name)
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
		Parameters m_mapParameters;
		Techniques m_listTechniques;
	};

	LPD3DXEFFECTPOOL CEffect::m_spPool = NULL;
	int CEffect::m_sNumEffects = 0;

	//------------------------------------------------------------------------------
	// Static methods.
	//------------------------------------------------------------------------------
	PEffect Effect::Create(const std::string& fileName)
	{
		//guard(PEffect Effect::Create(std::wstring fileName))

		EffectsList::iterator it = std::find_if(effects.begin(), effects.end(), _seacher(fileName));

		if (it != effects.end())
			return it->effect;

		EffectEntry ee;
		ee.effect = PEffect(new CEffect());
		ee.effect->load(fileName);
		ee.name = fileName;

		effects.push_back(ee);

		return ee.effect;

		//unguard
	}

	void Effect::ClearAll()
	{
		//guard(void Effect::ClearAll())
			effects.clear();
		//unguard
	}
}