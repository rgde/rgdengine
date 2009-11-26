#pragma once

//STD
#include <memory>
#include <string>
#include <exception>
#include <cassert>
#include <math.h>
#include <time.h>

//STL
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iomanip>
#include <algorithm>
#include <strstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

#include <gmtl/gmtl.h>

namespace math
{
	using namespace Math;
}

#define TIXML_USE_STL
#include "Tinyxml\\tinyxml.h"

//Boost
//#include <boost\\timer.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/tokenizer.hpp>
#include <boost/ref.hpp>
#define BOOST_FUNCTION_MAX_ARGS 3
#include <boost/function.hpp>
#include <boost/bind/apply.hpp>
#include <boost/call_traits.hpp>

typedef unsigned char uchar;
typedef	uchar		  byte;
typedef unsigned int  uint;

#include "base\\counter.h"
#include "base\\dator.h"
#include "base\\log.h"
#include "base\\log_helper.h"
#include "base\\exceptions.h"
#include "base\\lexical_cast.h"
#include "base\\manager.h"
#include "base\\multitree.h"
#include "base\\singelton.h"
#include "base\\tree.h"
#include "base\\smart_ptr_helpers.h"

#include "io\\io.h"

#include "base\\xml_helpers.h"

//Core
#include "core\\property.h"
#include "core\\named_object.h"
#include "core\\factory.h"
#include "core\\timer.h"
#include "core\\task.h"
#include "core\\coreComPtr.h"
#include "core\\application.h"
#include "core\\render_system.h"
#include "core\\render_task.h"
#include "core\\game_task.h"
#include "core\\input_task.h"
#include "core\\xml_class.h"
#include "core\\xml_node.h"

#include "event\\events.h"

#include "game\\game.h"

#include "input\\input.h"

#include "math\\track.h"
#include "math\\track_camera.h"
#include "math\\types3d.h"
#include "math\\transform.h"
#include "math\\random.h"
#include "math\\interpolyator.h"
#include "math\\frustum.h"
#include "math\\camera.h"
#include "math\\camera_controller.h"
#include "math\\fly_camera.h"
#include "math\\fps_camera.h"
#include "math\\target_camera.h"
#include "math\\animation_controller.h"
#include "math\\splines.h"
#include "math\\interpolators.h"

#include "render\\common.h"
#include "render\\font.h"
#include "render\\render_device.h"
#include "render\\texture.h"
#include "render\\effect.h"
#include "render\\material.h"
#include "render\\vertices.h"
#include "render\\geometry.h"
#include "render\\fog.h"
#include "render\\light.h"
#include "render\\light_manager.h"
#include "render\\render_target.h"
#include "render\\camera.h"
#include "render\\camera_manager.h"
#include "render\\binders.h"
#include "render\\manager.h"
#include "render\\lines3d.h"
#include "render\\lines2d.h"
#include "render\\mesh.h"
#include "render\\model.h"
#include "render\\sprites.h"
#include "render\\geom_generator.h"
#include "render\\flame_renderer.h"
#include "render\\lens_flares.h"
#include "render\\screen_fader.h"

#include "scene\\tree.h"
#include "scene\\scene.h"
#include "scene\\manager.h"
#include "scene\\base_trigger.h"
#include "scene\\distance_trigger.h"