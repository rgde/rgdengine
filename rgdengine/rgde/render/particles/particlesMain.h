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
#include <rgde/io/ioSerializedObject.h>

// render
#include <rgde/render/renderManager.h>
#include <rgde/render/renderEffect.h>
#include <rgde/render/renderDevice.h>
#include <rgde/render/renderTexture.h>
#include <rgde/render/renderGeometry.h>


// math
#include <rgde/math/mathTypes3d.h>
#include <rgde/math/mathTransform.h>
#include <rgde/math/mathInterpolyator.h>

// utility
#include <rgde/render/renderLines3d.h>

// game
#include <rgde/game/DynamicObject.h>