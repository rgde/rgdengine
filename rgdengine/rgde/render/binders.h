//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 12.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: Functions for creating two binders.
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <rgde/render/binder.h>

namespace render
{
	class material;
	typedef boost::shared_ptr<class effect> effect_ptr;

	dynamic_binder_ptr createDynamicBinder(const effect_ptr& effect,
										   const material& mat,
										   std::string& techniqueName);

	static_binder_ptr createStaticBinder(const effect_ptr& effect);
}