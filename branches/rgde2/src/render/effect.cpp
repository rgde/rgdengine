#include "stdafx.h"

#include <rgde/render/effect.h>

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
	base_handle::base_handle()
		: m_handle(0)
	{}

	base_handle::~base_handle(){}


	bool effect::m_skip_unused_params = true;

	//////////////////////////////////////////////////////////////////////////
	bool base_handle::is_annotation_exist(const std::string& annot_name) const 
	{
		return annotations.find(to_lower(annot_name)) != annotations.end();
	}

	const std::string& base_handle::get_annotation(const std::string& annot_name) const
	{
		annot_const_iter it = annotations.find(to_lower(annot_name));
		if (it!=annotations.end())
			return it->second;

		static const std::string empty_str;

		return empty_str;
	}



	tech_info::tech_info()
	{
		max_lights = 1; 
		per_pixel = true; 
		light_types = 0; 
		ps_version = 1.1f; 
		shadow_caster = false; 
		light_maping = false;
	}
	//////////////////////////////////////////////////////////////////////////
	param_info::ui_params::ui_params()
		: has_slider(false)
		, slider_max(10000)
		, slider_min(0)
		, slider_factor(1)
		, is_color_swatch(false) 
	{
	}

	void effect::fill_ui_params(std::list<param_ptr>& params_list)
	{
		int t = 0;
		std::list<param_ptr>::iterator paramIter = params_list.begin();
		while (paramIter != params_list.end())
		{
			param_info& param = **paramIter;
			annot_iter annIter = param.annotations.begin();
			while (annIter != param.annotations.end())
			{
				if (annIter->first == "UIName")
					param.m_ui_params.name = annIter->second;
				else
				if (annIter->first == "UIType")
				{
					if (annIter->second == "IntSpinner")
						param.m_ui_params.has_slider = true;
					else
					if (annIter->second == "ColorSwatch")
						param.m_ui_params.is_color_swatch = true;
				}
				else
				if (annIter->first == "UIMax")
				{
					int max = atoi(annIter->second.c_str());
					if (max > 0)
						param.m_ui_params.slider_max = max;
				}
				else
				if (annIter->first == "UIMin")
					param.m_ui_params.slider_min = atoi(annIter->second.c_str());
				else
				if (annIter->first == "UIStep")
				{
					int step = atoi(annIter->second.c_str());
					if (step > 0)
						param.m_ui_params.slider_factor = step;
				}
				++annIter;
			}

			++paramIter;
		}
	}

	void effect::erase_param_block(param_block_iter iter)
	{
		m_param_blocks.erase(iter);
	}

	effect::effect(void* effect, float shader_max_version)
		: m_platform_handle(effect)
	{
		shader_max_version = 2.2f;

		if (0 != m_platform_handle)
			init(shader_max_version);
	}


	namespace
	{
		struct is_expired
		{
			typedef boost::weak_ptr<param_block> paramblock_wptr;
			bool operator()(paramblock_wptr& pb)
			{
				return pb.expired();
			}
		};
	}

	void effect::clean_param_blocks()
	{
		typedef param_blocks::iterator PBIter;
		{
			PBIter it = std::remove_if(m_param_blocks.begin(), m_param_blocks.end(), is_expired());
			m_param_blocks.erase(it, m_param_blocks.end());
		}
		
		for (PBIter it = m_param_blocks.begin(); it != m_param_blocks.end(); ++it)
		{
			paramblock_ptr bp = it->lock();
			bp->free_handle();
		}
	}

//#ifdef _DEBUG
#define EFFECT_NAME_VALIDATE assert(to_lower(name) == name);
//#else
//#define EFFECT_NAME_VALIDATE
//#endif

	techinfo_ptr effect::get_tech(const std::string& name)
	{
		//EFFECT_NAME_VALIDATE
		tech_map::iterator it = m_techiques.find(to_lower(name));
		if (it != m_techiques.end())
			return it->second;
		else
			return techinfo_ptr();
	}

	param_ptr effect::get_param(const std::string& param_name) const
	{
		//EFFECT_NAME_VALIDATE
		params_iter it = m_params_by_name.find(param_name);

		if(it != m_params_by_name.end())
			return it->second;

		return param_ptr();
	}

	param_ptr effect::get_param_by_semantic(const std::string& param_semantic) const
	{
		//EFFECT_NAME_VALIDATE
		params_iter it = m_params_by_semantic.find(param_semantic);

		if(it != m_params_by_semantic.end())
			return it->second;

		return param_ptr();
	}

#define EFFECT_PARAMS_VALIDATE _ASSERTE((NULL != m_platform_handle) && param);

	bool effect::set(const std::string& param_name, float value)
	{
		if(param_ptr p = get_param(param_name))
		{
			return set(p, value);
		}
		return false;
	}

	bool effect::set(const std::string& param_name, texture_ptr value)
	{
		if(param_ptr p = get_param(param_name))
		{
			return set(p, value);
		}
		return false;
	}

	bool effect::set(const std::string& param_name, const math::mat44f& value)
	{
		if(param_ptr p = get_param(param_name))
		{
			return set(p, value);
		}
		return false;
	}

	bool effect::set(const std::string& param_name, const math::vec4f& value)
	{
		if(param_ptr p = get_param(param_name))
		{
			return set(p, value);
		}
		return false;
	}

	bool effect::set(const std::string& param_name, void* data, unsigned int bytes)
	{
		if(param_ptr p = get_param(param_name))
		{
			return set(p, data, bytes);
		}
		return false;
	}

	bool effect::set(const std::string& param_name, const float* pf, unsigned int count)
	{
		if(param_ptr p = get_param(param_name))
			return set(p, pf, count);

		return false;
	}

	bool effect::set(const std::string& param_name, int value)
	{
		if(param_ptr p = get_param(param_name))
			return set(p, value);

		return false;
	}

	bool effect::set(const std::string& param_name, bool value)
	{
		if(param_ptr p = get_param(param_name))
			return set(p, value);

		return false;
	}

	bool effect::set(const std::string& param_name, const math::vec4f* array, unsigned int count)
	{
		if(param_ptr p = get_param(param_name))
			return set(p, array, count);

		return false;
	}

	bool effect::set(const std::string& param_name, const math::mat44f* array, unsigned int count)
	{
		if(param_ptr p = get_param(param_name))
			return set(p, array, count);

		return false;
	}

	namespace
	{
		typedef boost::function<void(void)> unreg_func;
		void param_block_deleter(paramblock_ptr block, unreg_func func)
		{
			if (block && 0 != block->get_parent_effect())
			{
				func();
			}
		}
	}

	paramblock_ptr effect::create_paramblock()
	{
		paramblock_ptr temp_ptr(new param_block(*this));
		m_param_blocks.push_back(temp_ptr);

		param_block_iter iter = (--m_param_blocks.end());

		unreg_func func = boost::bind(&effect::erase_param_block, this, iter);

		paramblock_ptr block_ptr(
			temp_ptr.get(),
			boost::bind(&param_block_deleter, temp_ptr, func)
			);
		
		return block_ptr;
	}

	//////////////////////////////////////////////////////////////////////////

	param_block::param_block(effect& parent_effect) :
		m_handle(0), m_parent_effect(&parent_effect)
	{
		
	}

	param_block::~param_block()
	{
		free_handle();
	}

}
}
}