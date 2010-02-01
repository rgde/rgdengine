#pragma once

#include <list>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <boost/unordered_map.hpp>

#include "src/base.h"
#include "src/size.h"
#include "src/vector.h"
#include "src/rect.h"
#include "src/input.h"
#include "src/renderer.h"
#include "src/renderCallback.h"
#include "src/texture.h"
#include "src/image.h"
#include "src/font.h"
#include "src/imageset.h"
#include "src/cursor.h"
#include "src/texmanager.h"
#include "src/scripthost.h"
#include "src/system.h"
#include "src/window.h"

//controls
#include "src/panel.h"
#include "src/cstatictext.h"
#include "src/ceditbox.h"
#include "src/cbutton.h"
#include "src/ccheckbox.h"
#include "src/cframewindow.h"
#include "src/cprogress.h"
#include "src/cslider.h"
#include "src/cstaticimage.h"
#include "src/cscrollpane.h"
#include "src/clist.h"
#include "src/ccombobox.h"
#include "src/cmarkuptext.h"
