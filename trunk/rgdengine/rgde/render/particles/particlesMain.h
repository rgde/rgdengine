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
#include "io/ioSerializedObject.h"

// render
#include "render/renderManager.h"
#include "render/renderEffect.h"
#include "render/renderDevice.h"
#include "render/renderTexture.h"
#include "render/renderGeometry.h"


// math
#include "math/mathTypes3d.h"
#include "math/mathTransform.h"
#include "math/mathInterpolyator.h"

// utility
#include "render/renderLines3d.h"

// game
#include "game/DynamicObject.h"