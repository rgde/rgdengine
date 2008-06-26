/**--------------------------------------------------------------------------------------

									RGDE Engine

	-----------------------------------------------------------------------------
\author Denis V. Ovod, 2004

\author Andrew "Zlobnik" Chouprina, 2005
mail:	dxprg@mail.ru


Created:			2004 (Denis V. Ovod)
Modified for RGDE:	march-april 2005 (Zlobnik)
---------------------------------------------------------------------------------------*/
#pragma once

#include <assert.h>

// std
#include <vector> 
#include <list> 
#include <map>
#include <string>
#include <algorithm>
#include <functional>

// io
#include <rgde/io/serialized_object.h>

// render
#include <rgde/render/manager.h>
#include <rgde/render/effect.h>
#include <rgde/render/device.h>
#include <rgde/render/texture.h>
#include <rgde/render/geometry.h>


// math
#include <rgde/math/types3d.h>
#include <rgde/math/transform.h>
#include <rgde/math/interpolyator.h>

// utility
#include <rgde/render/lines3d.h>

// game
#include <rgde/game/dynamic_object.h>