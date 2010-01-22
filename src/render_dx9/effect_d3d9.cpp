#include "stdafx.h"

#include <rgde/render/effect.h>
#include "texture.h"

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
	//////////////////////////////////////////////////////////////////////////
	void param_info::refresh(effect& e)
	{
		ID3DXEffect* effect = (ID3DXEffect*)e.m_platform_handle;

		m_handle = 
			(internal_effect_handle)
				(effect ? effect->GetParameterByName(NULL, m_name.c_str()) : 0);
	}

	void tech_info::refresh(effect& e)
	{
		ID3DXEffect* effect = (ID3DXEffect*)e.m_platform_handle;

		m_handle = (internal_effect_handle)
			(effect ? effect->GetTechnique(tech_index) : 0);
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

	param_info::type get_param_type(D3DXPARAMETER_TYPE type, int cols, int rows)
	{
		switch(type)
		{
		case D3DXPT_INT:
			return param_info::INT;
		case D3DXPT_STRING:
			return param_info::STRING;
		case D3DXPT_FLOAT:
			if (rows == 1) // single var
			{
				if (cols == 1)
				{
					return param_info::FLOAT;
				}
				else if (cols == 2)
				{
					return param_info::FLOAT2;
				}
				else if (cols == 3)
				{
					return param_info::FLOAT3;
				}
				else if (cols == 4)
				{
					return param_info::FLOAT4;
				}
			}
			break;
		case D3DXPT_BOOL:
			return param_info::BOOL;
			break;
		case D3DXPT_TEXTURE:
		case D3DXPT_TEXTURE1D:
		case D3DXPT_TEXTURE2D:
		case D3DXPT_TEXTURECUBE:
		case D3DXPT_TEXTURE3D:
			return param_info::TEXTURE;
			break;

		default:				
			break;
		}
		return param_info::INVALID;
	}

	void effect::parse_techniques(std::list<techinfo_ptr>& tech_list, float shader_max_version)
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;

		D3DXEFFECT_DESC effect_desc;
		effect->GetDesc(&effect_desc);

		std::list<techinfo_ptr>& techniques = tech_list;

		for(uint i = 0; i < effect_desc.Techniques; ++i)
		{
			D3DXHANDLE tech_handle = effect->GetTechnique(i);
			D3DXTECHNIQUE_DESC tech_desc;
			effect->GetTechniqueDesc(tech_handle, &tech_desc);

			techinfo_ptr tech_ptr(new tech_info);

			tech_info& ti = *tech_ptr;
			ti.m_name = tech_desc.Name;
			ti.m_handle = (internal_effect_handle)tech_handle;
			ti.tech_index = i;
			ti.use_vs = false;

			ti.override_engine_multipass = false;
			if(tech_desc.Annotations)
			{
				D3DXHANDLE annotation = effect->GetAnnotationByName(tech_handle,"OverrideEngineMultipass");
				ti.override_engine_multipass = (NULL != annotation);
			}

			if (D3DXHANDLE hPass = effect->GetPass(tech_handle, 0))
			{
				D3DXPASS_DESC pass_desc;
				if(S_OK == effect->GetPassDesc(hPass, &pass_desc))
					ti.use_vs = (0 != pass_desc.pVertexShaderFunction);
			}

			{				
				D3DXPASS_DESC pDesc;
				effect->GetPassDesc(effect->GetPass(tech_handle,0),&pDesc);

				if(pDesc.pPixelShaderFunction)
				{
					DWORD ver = D3DXGetShaderVersion(pDesc.pPixelShaderFunction);
					if(ver == D3DPS_VERSION(3,0))
						ti.ps_version = 3.0f;
					if(ver == D3DPS_VERSION(2,1))
						ti.ps_version = 2.1f;
					if(ver == D3DPS_VERSION(2,2))
						ti.ps_version = 2.2f;
					if(ver == D3DPS_VERSION(2,0))
						ti.ps_version = 2.0f;
					if(ver == D3DPS_VERSION(1,4))
						ti.ps_version = 1.4f;
					if(ver == D3DPS_VERSION(1,3))
						ti.ps_version = 1.3f;
					if(ver == D3DPS_VERSION(1,2))
						ti.ps_version = 1.2f;
					if(ver == D3DPS_VERSION(1,1))
						ti.ps_version = 1.1f;
				}
			}

			for (uint annot_index = 0; annot_index < tech_desc.Annotations; ++annot_index)
			{
				D3DXHANDLE hAnnot = effect->GetAnnotation( ti.m_handle, annot_index );

				D3DXPARAMETER_DESC AnnotDesc;
				effect->GetParameterDesc( hAnnot, &AnnotDesc );

				if (D3DXPT_STRING == AnnotDesc.Type)
				{
					LPCSTR pstrName = NULL;
					effect->GetString( hAnnot, &pstrName );
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
					ti.light_types |= light_omni_proj;
				if(str.find("omni,") != -1 || str.find("point") != -1)
					ti.light_types |= light_omni;
				if(str.find("spot") != -1)
					ti.light_types |= light_spot;
				if(str.find("ambient") != -1 || str.find("dir") != -1)
					ti.light_types |= light_dir;

				if(ti.light_types == 0)
					ti.light_types = light_omni;
			}

			annot_it = ti.annotations.find("maxlights");
			if(annot_it != ti.annotations.end())
			{
				ti.max_lights = atoi(annot_it->second.c_str());
			}

			// Extract methods
			annot_it = ti.annotations.find("lightmethods");
			if(annot_it != ti.annotations.end())
			{
				std::string str = to_lower(annot_it->second);
				ti.per_pixel   = (str.find("perpixel") != -1);
				ti.shadow_caster   = (str.find("shadowproject") != -1);
				ti.light_maping = (str.find("lightmap") !=-1);
			}

			{//Tehcnique supports isntancing
				D3DXHANDLE handle = effect->GetParameterByName(NULL,"Instanced");
				ti.supports_instancing = handle && (TRUE == effect->IsParameterUsed(handle,ti.m_handle));
			}

			// Default if nothing specified
			if(ti.light_types == 0)
				ti.light_types = light_omni;

			// Technique annotation for optional grouping
			{
				annot_it = ti.annotations.find("techgroup");
				if(annot_it != ti.annotations.end())
					ti.group = to_lower(annot_it->second);
			}

			if (ti.ps_version <= shader_max_version)
			{
				techniques.push_back(tech_ptr);
			}
		}

		std::list<tech_list_iter> erase_items;
		
		for(tech_list_iter it = techniques.begin(); it != techniques.end(); ++it)
		{
			tech_info& a = *(*it);

			if (std::find(erase_items.begin(), erase_items.end(), it) != erase_items.end())
				continue;
			// Search for techniques inferior to a

			for(tech_list_iter j = techniques.begin(); j != techniques.end(); ++j)
			{
				if (std::find(erase_items.begin(), erase_items.end(), j) != erase_items.end())
					continue;

				tech_info& b = *(*j);

				if(a.m_handle == b.m_handle)
					continue;

				// A is superior to B in every way
				if(a.group == b.group && 
					b.light_types & a.light_types && 
					(
					(b.per_pixel/* && b.PRT*/)
					||
					((a.per_pixel || !b.per_pixel) &&
					/*(a.PRT || !b.PRT) && */
					(a.light_maping || !b.light_maping)) 
					)
					&& 
					a.max_lights >= b.max_lights &&
					a.ps_version >= b.ps_version
					&& (a.shadow_caster == b.shadow_caster))
				{
					erase_items.push_back(j);
				}
			}
		}
	}


	void effect::parse_params(std::list<param_ptr>& params_list)
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;

		D3DXEFFECT_DESC effect_desc;
		effect->GetDesc(&effect_desc);

		for (uint i = 0; i < effect_desc.Parameters; ++i)
		{
			param_ptr param(new param_info);
			param_info& pi = *param;
			pi.m_is_used = false;
			D3DXHANDLE param_handle = effect->GetParameter(NULL, i);
			assert(param_handle);

			pi.m_handle = (internal_effect_handle)param_handle;

			D3DXPARAMETER_DESC param_desc;
			effect->GetParameterDesc(param_handle, &param_desc);
			//m_effect->GetParameterDesc(param_handle, &pi.m_dx_desc);
			//D3DXPARAMETER_DESC& param_desc = pi.m_dx_desc;

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
				D3DXHANDLE hAnnot = effect->GetAnnotation( pi.m_handle, annot_index );

				D3DXPARAMETER_DESC AnnotDesc;
				effect->GetParameterDesc( hAnnot, &AnnotDesc );

				std::string annot_text;
				std::string annot_name = (AnnotDesc.Name);

				if (D3DXPT_STRING == AnnotDesc.Type)
				{
					LPCSTR pstrName = NULL;
					effect->GetString( hAnnot, &pstrName );
					annot_text = (pstrName);				
				}  else
				if (D3DXPT_INT == AnnotDesc.Type)
				{
					int val = 0;
					effect->GetInt( hAnnot, &val );
					annot_text = boost::lexical_cast<std::string>(val);
				} else
				if (D3DXPT_FLOAT == AnnotDesc.Type)
				{
					float val = 0;
					effect->GetFloat( hAnnot, &val );
					annot_text = boost::lexical_cast<std::string>(val);
				}
				pi.annotations.insert(annotations_map::value_type(annot_name, annot_text));
			}

			pi.m_is_tweakable = is_tweakable(pi.annotations);
			pi.m_type = get_param_type(param_desc.Type, param_desc.Columns, param_desc.Rows);

			if (pi.m_type != param_info::INVALID)
			{
				pi.m_data.resize(param_desc.Bytes);
				effect->GetValue(pi.m_handle,&(pi.m_data[0]),param_desc.Bytes);
			}

			params_list.push_back(param);
		}
	}

	void effect::init(float shader_max_version)
	{
		m_reads_color_buffer = false;
		m_reads_global_color_buffer = false;
		m_force_ldr = false;
		m_use_light_array = false;

		m_techniques_list.clear();
		tech_list& tech_list = m_techniques_list;
		parse_techniques(tech_list, shader_max_version);

		typedef std::list<param_ptr> ParamList;
		ParamList params_list;
		parse_params(params_list);
		fill_ui_params(params_list);

		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;

		//Read global params
		//for (ParamList::iterator pit = params_list.begin(); pit != params_list.end(); ++pit)
		{
			//param_ptr& param = (*pit);
			D3DXHANDLE global = effect->GetParameterBySemantic(NULL, "StandardsGlobal");
			if (global)
			{
				D3DXHANDLE hAnnot = effect->GetAnnotationByName( global, "Script");
				if (hAnnot)
				{
					D3DXPARAMETER_DESC AnnotDesc;
					effect->GetParameterDesc( hAnnot, &AnnotDesc );

					std::string annot_text;

					if (D3DXPT_STRING == AnnotDesc.Type)
					{
						LPCSTR pstrName = NULL;
						effect->GetString( hAnnot, &pstrName );
						annot_text = (pstrName);				
					}

					if(!annot_text.empty())
					{
						const std::string& str = annot_text;

						m_reads_color_buffer = str.find("ReadsColorBuffer") != -1;
						m_reads_global_color_buffer = str.find("ReadsGlobalColorBuffer") != -1;
						m_force_ldr = str.find("ForceLDR") != -1;
						m_use_light_array = str.find("UsesLightArray") != -1;
					}
				}
			}
		}


		m_handlers.reserve(params_list.size() + tech_list.size());

		for(tech_list::iterator it = tech_list.begin(); it != tech_list.end(); ++it)
		{
			internal_effect_handle tech_handle = (*it)->m_handle;
			m_handlers.push_back(*it);

			for (ParamList::iterator pit = params_list.begin(); pit != params_list.end(); ++pit)
			{
				param_ptr& param = (*pit);

				internal_effect_handle param_handle = param->m_handle;

				if (param_handle && TRUE == effect->IsParameterUsed(param_handle, tech_handle))
				{
					(*it)->params[to_lower(param->get_name())] = param;
					param->m_is_used = true;					
				}
			}

			m_techiques[to_lower((*it)->get_name())] = (*it);
		}

		m_params.reserve(params_list.size());

		for (ParamList::iterator pit = params_list.begin(); pit != params_list.end(); ++pit)
		{
			param_ptr& pptr = *pit;
			param_info& p = *pptr;

			if (p.m_is_used || !m_skip_unused_params)
			{
				m_handlers.push_back(pptr);
				m_params.push_back(pptr);
				m_params_by_name[to_lower(p.get_name())] = pptr;

				if (!p.get_semantic().empty())
				{
					m_params_by_semantic[to_lower(p.get_semantic())] = pptr;
				}
			}			
		}
	}

	effect::~effect()
	{
		clean_param_blocks();

		{
			typedef param_blocks::iterator PBIter;
			for (PBIter it = m_param_blocks.begin(); it != m_param_blocks.end(); ++it)
			{
				paramblock_ptr bp = it->lock();
				bp->m_parent_effect = NULL;
			}
		}

		typedef handles_vertor::iterator HIter;
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

		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;

		if (NULL != effect)
		{
			ULONG refs = effect->Release();			
			assert(0 == refs && "ULONG refs = m_effect->release()");
			effect = NULL;
		}
	}

	void effect::set_tech(techinfo_ptr tech)
	{
		D3DXHANDLE h = tech->m_handle;
		if (NULL == h) return;

		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;

		HRESULT hr = effect->SetTechnique(h);
		assert(hr == D3D_OK);
	}

	void effect::reload(void *new_effect)
	{
		clean_param_blocks();

		//if (NULL != new_effect)
		{
			if (NULL != m_platform_handle)
			{
				ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
				effect->Release();
			}
			
			m_platform_handle = new_effect;
		}		

		typedef handles_vertor::iterator HIter;
		for (HIter it = m_handlers.begin(); it != m_handlers.end(); ++it)
		{
			(*it)->refresh(*this);
		}
	}

//#ifdef _DEBUG
#define EFFECT_NAME_VALIDATE assert(to_lower(name) == name);
//#else
//#define EFFECT_NAME_VALIDATE
//#endif


#define EFFECT_PARAMS_VALIDATE _ASSERTE((NULL != m_platform_handle) && param);


	bool effect::set(const param_ptr& param, float value) 
	{
		EFFECT_PARAMS_VALIDATE
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		HRESULT hr = effect->SetFloat(param->m_handle, value);
		return D3D_OK == hr;
	}

	bool effect::set(const param_ptr& param, texture_ptr value) 
	{
		EFFECT_PARAMS_VALIDATE
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		HRESULT hr = S_FALSE;

		IDirect3DTexture9* dx_texure = value->get_impl()->get_dx_texture();
		hr = effect->SetTexture(param->m_handle, dx_texure);

		return D3D_OK == hr;
	}

	bool effect::set(const param_ptr& param, const math::mat44f& value) 
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = effect->SetMatrix(param->m_handle, (const D3DXMATRIX*)&value);
		return D3D_OK == hr;
	}

	bool effect::set(const param_ptr& param, const math::vec4f& value) 
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = effect->SetVector(param->m_handle, (const D3DXVECTOR4*)&value);
		return D3D_OK == hr;
	}

	bool effect::set(const param_ptr& param, bool value)
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = effect->SetBool(param->m_handle, value);
		return D3D_OK == hr;
	}

	bool effect::set(const param_ptr& param, void* data, unsigned int bytes)
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = effect->SetValue(param->m_handle, data, bytes);
		return D3D_OK == hr;
	}

	bool  effect::set(const param_ptr& param, const float* pf, unsigned int count)
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = effect->SetFloatArray(param->m_handle, pf, count);
		return D3D_OK == hr;
	}

	bool effect::set(const param_ptr& param, const math::vec4f* pVector, unsigned int count)
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = effect->SetVectorArray(param->m_handle, (const D3DXVECTOR4*)pVector, count);
		return D3D_OK == hr;
	}

	bool effect::set(const param_ptr& param, const math::mat44f* matrix, unsigned int count)
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = effect->SetMatrixArray(param->m_handle, (const D3DXMATRIX* )matrix, count);
		return D3D_OK == hr;
	}

	bool effect::set(const param_ptr& param, int value)
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		EFFECT_PARAMS_VALIDATE
		HRESULT hr = effect->SetInt(param->m_handle, value);
		return D3D_OK == hr;
	}

	void effect::on_device_reset()
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		effect->OnResetDevice();
	}

	void effect::on_device_lost()
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		clean_param_blocks();
		effect->OnLostDevice();
	}

	void effect::commit_changes()
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		effect->CommitChanges();		
	}

	bool effect::set_tech(const std::string& tech_name) 
	{
		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;

		tech_map_iter it = m_techiques.find(to_lower(tech_name));
		if (it != m_techiques.end())
		{
			HRESULT hr = effect->SetTechnique(it->second->m_handle);
			return hr == S_OK;
		}
		return false;
	}

	unsigned int effect::begin(unsigned int flags) 
	{
		unsigned int num_passes = 0;

		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;

		if (effect)
			effect->Begin(&num_passes, flags);

		return num_passes;
	}

	bool effect::begin_pass(unsigned int pass) 
	{
		HRESULT hr = S_FALSE;

		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;

		if (effect)
			hr = effect->BeginPass(pass);

		return D3D_OK == hr;
	}

	bool effect::end_pass() 
	{
		HRESULT hr = S_FALSE;

		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;
		
		if (effect)
			hr = effect->EndPass();

		return D3D_OK == hr;
	}

	bool effect::end() 
	{
		HRESULT hr = S_FALSE;

		ID3DXEffect* effect = (ID3DXEffect*)m_platform_handle;

		if (effect)
			hr = effect->End();

		return D3D_OK == hr;	
	}


	void param_block::free_handle()
	{
		if (NULL != m_handle)
		{			
			assert(0 !=m_parent_effect && 0 != m_parent_effect->m_platform_handle );
			ID3DXEffect* effect = (ID3DXEffect*)m_parent_effect->m_platform_handle;
			effect->DeleteParameterBlock(m_handle);
			m_handle = NULL;			
		}
	}

	void param_block::begin()
	{
		free_handle();		
		assert(0 !=m_parent_effect && 0 != m_parent_effect->m_platform_handle );
		ID3DXEffect* effect = (ID3DXEffect*)m_parent_effect->m_platform_handle;
		effect->BeginParameterBlock();
	}

	void param_block::end()
	{
		assert(NULL == m_handle);
		assert(0 !=m_parent_effect && 0 != m_parent_effect->m_platform_handle );
		ID3DXEffect* effect = (ID3DXEffect*)m_parent_effect->m_platform_handle;
		m_handle = (internal_effect_handle)effect->EndParameterBlock();
	}

	void param_block::apply()
	{
		if (NULL != m_handle && 0 !=m_parent_effect && 0 != m_parent_effect->m_platform_handle)
		{
			ID3DXEffect* effect = (ID3DXEffect*)m_parent_effect->m_platform_handle;
			effect->ApplyParameterBlock(m_handle);
		}
	}
}
}
}