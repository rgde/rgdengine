//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 12.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: Functions for creating two binders.
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "render/renderBinder.h"
#include "render/renderMaterial.h"

namespace render
{
	PDynamicBinder createDynamicBinder(const PEffect& pEffect,
									   const CMaterial& mat,
									   std::string& techniqueName);

	PStaticBinder createStaticBinder(const PEffect& pEffect);
}