#include "stdafx.h"

#include "effect_d3d9.h"

#include <boost/algorithm/string.hpp>

namespace 
{
	typedef unsigned int uint;

	std::string to_lower(const std::string& s)
	{
		return boost::algorithm::to_lower_copy(s);
	}
}

namespace rgde
{
namespace render
{
namespace effects
{

	bool Effect::skip_unused_params = true;

	//////////////////////////////////////////////////////////////////////////
	bool BaseHandle::IsAnnotationExist(const std::string& annot_name) const 
	{
		return annotations.find(to_lower(annot_name)) != annotations.end();
	}

	std::string BaseHandle::GetAnnotation(const std::string& annot_name) const
	{
		annot_const_iter it = annotations.find(to_lower(annot_name));
		if (it!=annotations.end())
			return it->second;

		return std::string();
	}

	void ParamInfo::Refresh(Effect& e)
	{
		m_handle = 
			(internal_effect_handle)
				(e.m_effect ? e.m_effect->GetParameterByName(NULL, m_name.c_str()) : 0);
	}

	void TechInfo::Refresh(Effect& e)
	{
		m_handle = (internal_effect_handle)
			(e.m_effect ? e.m_effect->GetTechnique(tech_index) : 0);
	}

	TechInfo::TechInfo()
	{
		MaxLights = 1; 
		PerPixel = true; 
		LightTypes = 0; 
		PixelShaderVersion = 1.1f; 
		ShadowProjector = false; 
		LightMapping = false;
	}
	//////////////////////////////////////////////////////////////////////////
	namespace 
	{
		bool is_tweakable(const annotations_map& annotations)
		{
			// Material parameters are only tweakable things, and tweakable things 
			// always have annotations, so skip this if it's not annotated.
			if(annotations.empty())
				return false;

			// Check widget
			annot_const_iter it = annotations.find("UIWidget");
			if(it != annotations.end())
			{
				const std::string& annot_name = it->second;
				// No widget, thus not displayed
				if (annot_name.find("none") != -1 || annot_name.find("None") != -1)
					return false;
			}

			// Object annotations are not editable. They are system things such as lights
			if(annotations.find("Object") != annotations.end())
				return false;

			return true;
		}

	}

		ParamInfo::UIParams::UIParams()
			:HasSlider(false),
			SliderMax(10000),
			SliderMin(0),
			SliderFactor(1),
			isColorSwatch(false) 
		{
		}

		ParamInfo::Type get_param_type(D3DXPARAMETER_TYPE type, int cols, int rows)
		{
			switch(type)
			{
			case D3DXPT_INT:
				return ParamInfo::INT;
			case D3DXPT_STRING:
				return ParamInfo::STRING;
			case D3DXPT_FLOAT:
				if (rows == 1) // single var
				{
					if (cols == 1)
					{
						return ParamInfo::FLOAT;
					}
					else if (cols == 2)
					{
						return ParamInfo::FLOAT2;
					}
					else if (cols == 3)
					{
						return ParamInfo::FLOAT3;
					}
					else if (cols == 4)
					{
						return ParamInfo::FLOAT4;
					}
				}
				break;
			case D3DXPT_BOOL:
				return ParamInfo::BOOL;
				break;
			case D3DXPT_TEXTURE:
			case D3DXPT_TEXTURE1D:
			case D3DXPT_TEXTURE2D:
			case D3DXPT_TEXTURECUBE:
			case D3DXPT_TEXTURE3D:
				return ParamInfo::TEXTURE;
				break;

			default:				
				break;
			}
			return ParamInfo::INVALID;
		}

	void Effect::parse_techniques(std::list<TechInfoPtr>& tech_list, float shader_max_version)
	{
		D3DXEFFECT_DESC effect_desc;
		m_effect->GetDesc(&effect_desc);

		std::list<TechInfoPtr>& techniques = tech_list;

		for(uint i = 0; i < effect_desc.Techniques; ++i)
		{
			D3DXHANDLE tech_handle = m_effect->GetTechnique(i);
			D3DXTECHNIQUE_DESC tech_desc;
			m_effect->GetTechniqueDesc(tech_handle, &tech_desc);

			TechInfoPtr tech_ptr(new TechInfo);

			TechInfo& ti = *tech_ptr;
			ti.m_name = tech_desc.Name;
			ti.m_handle = (internal_effect_handle)tech_handle;
			ti.tech_index = i;
			ti.UseVS = false;

			ti.OverridesEngineMultipass = false;
			if(tech_desc.Annotations)
			{
				D3DXHANDLE annotation = m_effect->GetAnnotationByName(tech_handle,"OverrideEngineMultipass");
				ti.OverridesEngineMultipass = (NULL != annotation);
			}

			if (D3DXHANDLE hPass = m_effect->GetPass(tech_handle, 0))
			{
				D3DXPASS_DESC pass_desc;
				if(S_OK == m_effect->GetPassDesc(hPass, &pass_desc))
					ti.UseVS = (0 != pass_desc.pVertexShaderFunction);
			}

			{				
				D3DXPASS_DESC pDesc;
				m_effect->GetPassDesc(m_effect->GetPass(tech_handle,0),&pDesc);

				if(pDesc.pPixelShaderFunction)
				{
					DWORD ver = D3DXGetShaderVersion(pDesc.pPixelShaderFunction);
					if(ver == D3DPS_VERSION(3,0))
						ti.PixelShaderVersion = 3.0f;
					if(ver == D3DPS_VERSION(2,1))
						ti.PixelShaderVersion = 2.1f;
					if(ver == D3DPS_VERSION(2,2))
						ti.PixelShaderVersion = 2.2f;
					if(ver == D3DPS_VERSION(2,0))
						ti.PixelShaderVersion = 2.0f;
					if(ver == D3DPS_VERSION(1,4))
						ti.PixelShaderVersion = 1.4f;
					if(ver == D3DPS_VERSION(1,3))
						ti.PixelShaderVersion = 1.3f;
					if(ver == D3DPS_VERSION(1,2))
						ti.PixelShaderVersion = 1.2f;
					if(ver == D3DPS_VERSION(1,1))
						ti.PixelShaderVersion = 1.1f;
				}
			}

			for (uint annot_index = 0; annot_index < tech_desc.Annotations; ++annot_index)
			{
				D3DXHANDLE hAnnot = m_effect->GetAnnotation ( ti.m_handle, annot_index );

				D3DXPARAMETER_DESC AnnotDesc;
				m_effect->GetParameterDesc( hAnnot, &AnnotDesc );

				if (D3DXPT_STRING == AnnotDesc.Type)
				{
					LPCSTR pstrName = NULL;
					m_effect->GetString( hAnnot, &pstrName );
					std::string annot_text = to_lower(pstrName);
					std::string annot_name = to_lower(AnnotDesc.Name);

					ti.annotations.insert(annotations_map::value_type(annot_name, annot_text));
				}
			}

			annot_iter annot_it = ti.annotations.find("lighttypes");
			if (annot_it != ti.annotations.end())
			{
				const std::string& str = annot_it->second;

				if(str.find("omniprojector") != -1)
					ti.LightTypes |= LIGHT_OMNI_PROJ;
				if(str.find("omni,") != -1 || str.find("point") != -1)
					ti.LightTypes |= LIGHT_OMNI;
				if(str.find("spot") != -1)
					ti.LightTypes |= LIGHT_SPOT;
				if(str.find("ambient") != -1 || str.find("dir") != -1)
					ti.LightTypes |= LIGHT_DIR;

				if(ti.LightTypes == 0)
					ti.LightTypes = LIGHT_OMNI;
			}

			annot_it = ti.annotations.find("maxlights");
			if(annot_it != ti.annotations.end())
			{
				ti.MaxLights = atoi(annot_it->second.c_str());
			}

			// Extract methods
			annot_it = ti.annotations.find("lightmethods");
			if(annot_it != ti.annotations.end())
			{
				std::string str = to_lower(annot_it->second);
				ti.PerPixel   = (str.find("perpixel") != -1);
				ti.ShadowProjector   = (str.find("shadowproject") != -1);
				ti.LightMapping = (str.find("lightmap") !=-1);
			}

			{//Tehcnique supports isntancing
				D3DXHANDLE handle = m_effect->GetParameterByName(NULL,"Instanced");
				ti.SupportsInstancing = handle && (TRUE == m_effect->IsParameterUsed(handle,ti.m_handle));
			}

			// Default if nothing specified
			if(ti.LightTypes == 0)
				ti.LightTypes = LIGHT_OMNI;

			// Technique annotation for optional grouping
			{
				annot_it = ti.annotations.find("techgroup");
				if(annot_it != ti.annotations.end())
					ti.Group = to_lower(annot_it->second);
			}

			if (ti.PixelShaderVersion <= shader_max_version)
			{
				techniques.push_back(tech_ptr);
			}
		}

		typedef std::list<TechInfoPtr> TechList;
		typedef TechList::iterator TechListIter;

		std::list<TechListIter> erase_items;
		
		for(TechListIter it = techniques.begin(); it != techniques.end(); ++it)
		{
			TechInfo& a = *(*it);

			if (std::find(erase_items.begin(), erase_items.end(), it) != erase_items.end())
				continue;
			// Search for techniques inferior to a

			for(TechListIter j = techniques.begin(); j != techniques.end(); ++j)
			{
				if (std::find(erase_items.begin(), erase_items.end(), j) != erase_items.end())
					continue;

				TechInfo& b = *(*j);

				if(a.m_handle == b.m_handle)
					continue;

				// A is superior to B in every way
				if(a.Group == b.Group && 
					b.LightTypes & a.LightTypes && 
					(
					(b.PerPixel/* && b.PRT*/)
					||
					((a.PerPixel || !b.PerPixel) &&
					/*(a.PRT || !b.PRT) && */
					(a.LightMapping || !b.LightMapping)) 
					)
					&& 
					a.MaxLights >= b.MaxLights &&
					a.PixelShaderVersion >= b.PixelShaderVersion
					&& (a.ShadowProjector == b.ShadowProjector))
				{
					erase_items.push_back(j);
				}
			}
		}
	}


	void Effect::parse_params(std::list<ParamPtr>& params_list)
	{
		D3DXEFFECT_DESC effect_desc;
		m_effect->GetDesc(&effect_desc);

		for (uint i = 0; i < effect_desc.Parameters; ++i)
		{
			ParamPtr param(new ParamInfo);
			ParamInfo& pi = *param;
			pi.m_is_used = false;
			D3DXHANDLE param_handle = m_effect->GetParameter(NULL, i);
			assert(param_handle);

			pi.m_handle = (internal_effect_handle)param_handle;

			
			m_effect->GetParameterDesc(param_handle, &pi.m_dx_desc);
			D3DXPARAMETER_DESC& param_desc = pi.m_dx_desc;

			if (param_desc.Type == D3DXPT_SAMPLER ||
				param_desc.Type == D3DXPT_SAMPLER1D ||
				param_desc.Type == D3DXPT_SAMPLER2D ||
				param_desc.Type == D3DXPT_SAMPLER3D ||
				param_desc.Type == D3DXPT_SAMPLERCUBE ||

				param_desc.Type == D3DXPT_PIXELSHADER ||
				param_desc.Type == D3DXPT_VERTEXSHADER ||

				param_desc.Type == D3DXPT_PIXELFRAGMENT ||
				param_desc.Type == D3DXPT_VERTEXFRAGMENT/* ||
														param_desc.Type == D3DXPT_UNSUPPORTED*/
														)
														continue;

			pi.m_columns = param_desc.Columns;
			pi.m_name = param_desc.Name;

			if (NULL != param_desc.Semantic)
			{
				pi.m_semantic = param_desc.Semantic;
			}

			for (uint annot_index = 0; annot_index < param_desc.Annotations; ++annot_index)
			{
				D3DXHANDLE hAnnot = m_effect->GetAnnotation ( pi.m_handle, annot_index );

				D3DXPARAMETER_DESC AnnotDesc;
				m_effect->GetParameterDesc( hAnnot, &AnnotDesc );

				std::string annot_text;
				std::string annot_name = (AnnotDesc.Name);

				if (D3DXPT_STRING == AnnotDesc.Type)
				{
					LPCSTR pstrName = NULL;
					m_effect->GetString( hAnnot, &pstrName );
					annot_text = (pstrName);				
				}  else
				if (D3DXPT_INT == AnnotDesc.Type)
				{
					int val = 0;
					m_effect->GetInt( hAnnot, &val );
					annot_text = boost::lexical_cast<std::string>(val);
				} else
				if (D3DXPT_FLOAT == AnnotDesc.Type)
				{
					float val = 0;
					m_effect->GetFloat( hAnnot, &val );
					annot_text = boost::lexical_cast<std::string>(val);
				}
				pi.annotations.insert(annotations_map::value_type(annot_name, annot_text));
			}

			pi.m_is_tweakable = is_tweakable(pi.annotations);
			pi.m_type = get_param_type(param_desc.Type, param_desc.Columns, param_desc.Rows);

			if (pi.m_type != ParamInfo::INVALID)
			{
				pi.m_data.resize(param_desc.Bytes);
				m_effect->GetValue(pi.m_handle,&(pi.m_data[0]),param_desc.Bytes);
			}

			params_list.push_back(param);
		}
	}

	void Effect::FillUIParams(std::list<ParamPtr>& params_list)
	{
		int t = 0;
		std::list<ParamPtr>::iterator paramIter = params_list.begin();
		while (paramIter != params_list.end())
		{
			ParamInfo& param = **paramIter;
			annot_iter annIter = param.annotations.begin();
			while (annIter != param.annotations.end())
			{
				if (annIter->first == "UIName")
					param.m_ui_params.Name = annIter->second;
				else
				if (annIter->first == "UIType")
				{
					if (annIter->second == "IntSpinner")
						param.m_ui_params.HasSlider = true;
					else
					if (annIter->second == "ColorSwatch")
						param.m_ui_params.isColorSwatch = true;
				}
				else
				if (annIter->first == "UIMax")
				{
					int max = atoi(annIter->second.c_str());
					if (max > 0)
						param.m_ui_params.SliderMax = max;
				}
				else
				if (annIter->first == "UIMin")
					param.m_ui_params.SliderMin = atoi(annIter->second.c_str());
				else
				if (annIter->first == "UIStep")
				{
					int step = atoi(annIter->second.c_str());
					if (step > 0)
						param.m_ui_params.SliderFactor = step;
				}
				++annIter;
			}

			++paramIter;

		}

	}

	void Effect::erase_param_block(ParamBlockIter iter)
	{
		m_param_blocks.erase(iter);
	}

	void Effect::init(float shader_max_version)
	{
		m_ReadsColorBuffer = false;
		m_ReadsGlobalColorBuffer = false;
		m_ForceLDR = false;
		m_UsesLightArray = false;

		m_techniques_list.clear();
		TechList& tech_list = m_techniques_list;
		parse_techniques(tech_list, shader_max_version);

		typedef std::list<ParamPtr> ParamList;
		ParamList params_list;
		parse_params(params_list);
		FillUIParams(params_list);

		//Read global params
		//for (ParamList::iterator pit = params_list.begin(); pit != params_list.end(); ++pit)
		{
			//ParamPtr& param = (*pit);
			D3DXHANDLE global = m_effect->GetParameterBySemantic(NULL, "StandardsGlobal");
			if (global)
			{
				D3DXHANDLE hAnnot = m_effect->GetAnnotationByName( global, "Script");
				if (hAnnot)
				{
					D3DXPARAMETER_DESC AnnotDesc;
					m_effect->GetParameterDesc( hAnnot, &AnnotDesc );

					std::string annot_text;

					if (D3DXPT_STRING == AnnotDesc.Type)
					{
						LPCSTR pstrName = NULL;
						m_effect->GetString( hAnnot, &pstrName );
						annot_text = (pstrName);				
					}

					if(!annot_text.empty())
					{
						const std::string& str = annot_text;

						m_ReadsColorBuffer = str.find("ReadsColorBuffer") != -1;
						m_ReadsGlobalColorBuffer = str.find("ReadsGlobalColorBuffer") != -1;
						m_ForceLDR = str.find("ForceLDR") != -1;
						m_UsesLightArray = str.find("UsesLightArray") != -1;
					}
				}
			}
		}


		m_handlers.reserve(params_list.size() + tech_list.size());

		for(TechList::iterator it = tech_list.begin(); it != tech_list.end(); ++it)
		{
			internal_effect_handle tech_handle = (*it)->m_handle;
			m_handlers.push_back(*it);

			for (ParamList::iterator pit = params_list.begin(); pit != params_list.end(); ++pit)
			{
				ParamPtr& param = (*pit);

				internal_effect_handle param_handle = param->m_handle;

				if (param_handle && TRUE == m_effect->IsParameterUsed(param_handle, tech_handle))
				{
					(*it)->params[to_lower(param->GetName())] = param;
					param->m_is_used = true;					
				}
			}

			m_techiques[to_lower((*it)->GetName())] = (*it);
		}

		m_params.reserve(params_list.size());

		for (ParamList::iterator pit = params_list.begin(); pit != params_list.end(); ++pit)
		{
			ParamPtr& pptr = *pit;
			ParamInfo& p = *pptr;

			if (p.m_is_used || !skip_unused_params)
			{
				m_handlers.push_back(pptr);
				m_params.push_back(pptr);
				m_params_by_name[to_lower(p.GetName())] = pptr;

				if (!p.GetSemantic().empty())
				{
					m_params_by_semantic[to_lower(p.GetSemantic())] = pptr;
				}
			}			
		}
	}

	Effect::Effect(ID3DXEffect* effect, float shader_max_version)
		: m_effect(effect)
	{
		shader_max_version = 2.2f;

		if (NULL != m_effect)
			init(shader_max_version);
	}

	Effect::~Effect()
	{
		clean_param_blocks();

		{
			typedef ParamBlocks::iterator PBIter;
			for (PBIter it = m_param_blocks.begin(); it != m_param_blocks.end(); ++it)
			{
				ParamBlockPtr bp = it->lock();
				bp->m_parent_effect = NULL;
			}
		}

		typedef BaseHandlesVertor::iterator HIter;
		for (HIter it = m_handlers.begin(); it != m_handlers.end(); ++it)
		{
			(*it)->m_handle = NULL;
		}

		m_params.clear();
		m_params_by_name.clear();
		m_params_by_semantic.clear();
		m_param_blocks.clear();
		m_techniques_list.clear();
		m_techiques.clear();
		m_handlers.clear();

		if (NULL != m_effect)
		{
			ULONG refs = m_effect->Release();			
			assert(0 == refs && "ULONG refs = m_effect->release()");
			m_effect = NULL;
		}
	}

	namespace
	{
		struct is_expired
		{
			typedef boost::weak_ptr<ParamBlock> ParamBlockWeakPtr;
			bool operator()(ParamBlockWeakPtr& pb)
			{
				return pb.expired();
			}
		};
	}

	void Effect::clean_param_blocks()
	{
		typedef ParamBlocks::iterator PBIter;
		{
			PBIter it = std::remove_if(m_param_blocks.begin(), m_param_blocks.end(), is_expired());
			m_param_blocks.erase(it, m_param_blocks.end());
		}
		
		for (PBIter it = m_param_blocks.begin(); it != m_param_blocks.end(); ++it)
		{
			ParamBlockPtr bp = it->lock();
			bp->DeleteHandle();
		}
	}

	void Effect::SetTechnique(TechInfoPtr tech)
	{
		D3DXHANDLE h = tech->m_handle;
		if (NULL == h) return;
		HRESULT hr = m_effect->SetTechnique(h);
		assert(hr == D3D_OK);
	}

	void Effect::Reload(ID3DXEffect *new_effect)
	{
		clean_param_blocks();

		//if (NULL != new_effect)
		{
			if (NULL != m_effect)
			{
				m_effect->Release();
			}
			
			m_effect = new_effect;
		}		

		typedef BaseHandlesVertor::iterator HIter;
		for (HIter it = m_handlers.begin(); it != m_handlers.end(); ++it)
		{
			(*it)->Refresh(*this);
		}
	}

//#ifdef _DEBUG
#define EFFECT_NAME_VALIDATE assert(to_lower(name) == name);
//#else
//#define EFFECT_NAME_VALIDATE
//#endif

	TechInfoPtr Effect::GetTechnique(const std::string& name)
	{
		//EFFECT_NAME_VALIDATE
		TechMap::iterator it = m_techiques.find(to_lower(name));
		if (it != m_techiques.end())
			return it->second;
		else
			return TechInfoPtr();
	}

	ParamPtr Effect::GetParameterByName(const std::string& name) const
	{
		EFFECT_NAME_VALIDATE
		ParamsIter it = m_params_by_name.find(name);

		if(it != m_params_by_name.end())
			return it->second;

		return ParamPtr();
	}

	ParamPtr Effect::GetParameterBySemantic(const std::string& name) const
	{
		EFFECT_NAME_VALIDATE
		ParamsIter it = m_params_by_semantic.find(name);

		if(it != m_params_by_semantic.end())
			return it->second;

		return ParamPtr();
	}

#define EFFECT_PARAMS_VALIDATE _ASSERTE((NULL != m_effect) && param);


	bool Effect::SetFloat(const ParamPtr& param, float value) 
	{
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = m_effect->SetFloat(param->m_handle, value);
		return D3D_OK == hr;
	}

	bool Effect::SetTexture(const ParamPtr& param, IDirect3DBaseTexture9* value) 
	{
		EFFECT_PARAMS_VALIDATE		
		HRESULT hr = m_effect->SetTexture(param->m_handle, value);
		return D3D_OK == hr;
	}

	bool Effect::SetMatrix(const ParamPtr& param, const D3DXMATRIX& value) 
	{
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = m_effect->SetMatrix(param->m_handle, &value);
		return D3D_OK == hr;
	}

	bool Effect::SetVector(const ParamPtr& param, const D3DXVECTOR4& value) 
	{
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = m_effect->SetVector(param->m_handle, &value);
		return D3D_OK == hr;
	}

	bool Effect::SetBool(const ParamPtr& param, bool value)
	{
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = m_effect->SetBool(param->m_handle, value);
		return D3D_OK == hr;
	}

	bool Effect::SetValue(const ParamPtr& param, void* data, unsigned int bytes)
	{
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = m_effect->SetValue(param->m_handle, data, bytes);
		return D3D_OK == hr;
	}

	bool  Effect::SetFloatArray(const ParamPtr& param, const float* pf, unsigned int Count)
	{
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = m_effect->SetFloatArray(param->m_handle, pf, Count);
		return D3D_OK == hr;
	}

	bool Effect::SetVectorArray(const ParamPtr& param, const D3DXVECTOR4* pVector, unsigned int Count)
	{
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = m_effect->SetVectorArray(param->m_handle, pVector, Count);
		return D3D_OK == hr;
	}

	bool Effect::set_matrix_array(const ParamPtr& param, const D3DXMATRIX* matrix, unsigned int count)
	{
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = m_effect->SetMatrixArray(param->m_handle, matrix, count);
		return D3D_OK == hr;
	}

	bool Effect::SetInt(const ParamPtr& param, int value)
	{
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = m_effect->SetInt(param->m_handle, value);
		return D3D_OK == hr;
	}


	bool Effect::SetFloat(const std::string& param_name, float value)
	{
		if(ParamPtr p = GetParameterByName(param_name))
		{
			return SetFloat(p, value);
		}
		return false;
	}

	bool Effect::SetTexture(const std::string& param_name, IDirect3DBaseTexture9* value)
	{
		if(ParamPtr p = GetParameterByName(param_name))
		{
			return SetTexture(p, value);
		}
		return false;
	}

	bool Effect::SetMatrix(const std::string& param_name, const D3DXMATRIX& value)
	{
		if(ParamPtr p = GetParameterByName(param_name))
		{
			return SetMatrix(p, value);
		}
		return false;
	}

	bool Effect::SetVector(const std::string& param_name, const D3DXVECTOR4& value)
	{
		if(ParamPtr p = GetParameterByName(param_name))
		{
			return SetVector(p, value);
		}
		return false;
	}

	bool Effect::SetValue(const std::string& param_name, void* data, unsigned int bytes)
	{
		if(ParamPtr p = GetParameterByName(param_name))
		{
			return SetValue(p, data, bytes);
		}
		return false;
	}

	bool Effect::SetFloatArray(const std::string& param_name, const float* pf, unsigned int Count)
	{
		if(ParamPtr p = GetParameterByName(param_name))
			return SetFloatArray(p, pf, Count);

		return false;
	}

	bool Effect::SetInt(const std::string& param_name, int value)
	{
		if(ParamPtr p = GetParameterByName(param_name))
			return SetInt(p, value);

		return false;
	}

	bool Effect::SetBool(const std::string& param_name, bool value)
	{
		if(ParamPtr p = GetParameterByName(param_name))
			return SetBool(p, value);

		return false;
	}

	bool Effect::SetVectorArray(const std::string& param_name, const D3DXVECTOR4* pVector, unsigned int Count)
	{
		if(ParamPtr p = GetParameterByName(param_name))
			return SetVectorArray(p, pVector,Count);

		return false;
	}

	bool Effect::set_matrix_array(const std::string& param_name, const D3DXMATRIX* array, unsigned int count)
	{
		if(ParamPtr p = GetParameterByName(param_name))
			return set_matrix_array(p, array,count);

		return false;
	}

	void Effect::on_device_reset()
	{
		m_effect->OnResetDevice();
	}

	void Effect::on_device_lost()
	{
		clean_param_blocks();
		m_effect->OnLostDevice();
	}

	void Effect::commit_changes()
	{
		m_effect->CommitChanges();		
	}

	bool Effect::SetTechnique(const std::string& tech_name) 
	{
		TechMapIter it = m_techiques.find(to_lower(tech_name));
		if (it != m_techiques.end())
		{
			HRESULT hr = m_effect->SetTechnique(it->second->m_handle);
			return hr == S_OK;
		}
		return false;
	}

	unsigned int Effect::Begin(unsigned int flags) 
	{
		unsigned int num_passes = 0;

		if (NULL != m_effect)
			m_effect->Begin(&num_passes, flags);

		return num_passes;
	}

	bool Effect::BeginPass(unsigned int pass) 
	{
		if (NULL == m_effect)
			return false;

		HRESULT hr = m_effect->BeginPass(pass);
		return D3D_OK == hr;
	}

	bool Effect::EndPass() 
	{
		if (NULL == m_effect)
			return false;

		HRESULT hr = m_effect->EndPass();
		return D3D_OK == hr;
	}

	bool Effect::End() 
	{
		if (NULL == m_effect)
			return false;

		HRESULT hr = m_effect->End();
		return D3D_OK == hr;	
	}

	namespace
	{
		typedef boost::function<void(void)> unreg_func;
		void param_block_deleter(ParamBlockPtr block, unreg_func func)
		{
			if (block && NULL != block->GetParentEffect())
			{
				func();
			}
		}
	}

	ParamBlockPtr Effect::CreateParamBlock()
	{
		ParamBlockPtr temp_ptr(new ParamBlock(*this));
		m_param_blocks.push_back(temp_ptr);

		ParamBlockIter iter = (--m_param_blocks.end());

		unreg_func func = boost::bind(&Effect::erase_param_block, this, iter);

		ParamBlockPtr block_ptr(
			temp_ptr.get(),
			boost::bind(&param_block_deleter, temp_ptr, func)
			);
		
		return block_ptr;
	}

	//////////////////////////////////////////////////////////////////////////

	ParamBlock::ParamBlock(Effect& parent_effect) :
		m_handle(NULL), m_parent_effect(&parent_effect)
	{
		
	}

	ParamBlock::~ParamBlock()
	{
		DeleteHandle();
	}

	void ParamBlock::DeleteHandle()
	{
		if (NULL != m_handle)
		{
			assert(m_parent_effect != NULL && m_parent_effect->m_effect != NULL);
			m_parent_effect->m_effect->DeleteParameterBlock(m_handle);
			m_handle = NULL;			
		}
	}

	void ParamBlock::Begin()
	{
		DeleteHandle();
		assert(m_parent_effect != NULL && m_parent_effect->m_effect != NULL);
		m_parent_effect->m_effect->BeginParameterBlock();
	}

	void ParamBlock::End()
	{
		assert(NULL == m_handle);
		assert(m_parent_effect != NULL && m_parent_effect->m_effect != NULL);
		m_handle = (internal_effect_handle)m_parent_effect->m_effect->EndParameterBlock();
	}

	void ParamBlock::Apply()
	{
		if (NULL != m_handle && m_parent_effect != NULL && NULL != m_parent_effect->m_effect)
			m_parent_effect->m_effect->ApplyParameterBlock(m_handle);
	}
}
}
}