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
	PDynamicBinder createDynamicBinder(const PEffect& pEffect,
									   const Material& mat,
									   std::string& techniqueName);

	PStaticBinder createStaticBinder(const PEffect& pEffect);
}