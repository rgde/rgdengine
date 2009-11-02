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
#include <rgde/render/material.h>

namespace render
{
	dynamic_binder_ptr createDynamicBinder(const effect_ptr& pEffect,
									   const material& mat,
									   std::string& techniqueName);

	PStaticBinder createStaticBinder(const effect_ptr& pEffect);
}