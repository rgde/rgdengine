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
	using namespace gmtl;
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
#include "base\\logHelper.h"
#include "base\\exceptions.h"
#include "base\\hash_string.h"
#include "base\\lexical_cast.h"
#include "base\\manager.h"
#include "base\\multitree.h"
#include "base\\singelton.h"
#include "base\\tree.h"
//#include "base\\visitor.h"
#include "base\\smart_ptr_helpers.h"

#include "io\\io.h"

#include "base\\xml_helpers.h"

//Core
#include "core\\Property.h"
#include "core\\NamedObject.h"
#include "core\\factory.h"
#include "core\\Timer.h"
#include "core\\Task.h"
#include "core\\coreComPtr.h"
#include "core\\application.h"
#include "core\\coreRenderDevice.h"
#include "core\\RenderTask.h"
#include "core\\GameTask.h"
#include "core\\InputTask.h"
#include "core\\xmlClass.h"
#include "core\\xmlNode.h"

#include "event\\events.h"

#include "game\\game.h"

#include "input\\base.h"
#include "input\\input.h"
#include "input\\inputimpl.h"
#include "input\\device.h"
#include "input\\control.h"
#include "input\\command.h"
#include "input\\helper.h"

#include "math\\mathTrack.h"
#include "math\\mathTrackCamera.h"
#include "math\\mathTypes3d.h"
#include "math\\mathTransform.h"
#include "math\\mathRandom.h"
#include "math\\mathInterpolyator.h"
#include "math\\mathFrustum.h"
#include "math\\mathCamera.h"
#include "math\\mathCameraController.h"
#include "math\\mathFlyCamera.h"
#include "math\\mathFirstPersonCamera.h"
#include "math\\mathTargetCamera.h"
#include "math\\mathAnimationController.h"
#include "math\\mathSplines.h"
#include "math\\mathInterpolators.h"

//#include "sound\\soundDirectAudioTypes.h"
//#include "sound\\soundAbstractSound.h"
//#include "sound\\soundAbstractManager.h"
//#include "sound\\soundLoader.h"
//#include "sound\\soundWaveFile.h"
//#include "sound\\sound3d.h"
//#include "sound\\soundMusic.h"
//#include "sound\\sound3dManager.h"
//#include "sound\\soundMusicManager.h"
//#include "sound\\soundFrameSoundManager.h"

#include "render\\renderCommon.h"
#include "render\\renderFont.h"
#include "render\\renderDevice.h"
#include "render\\renderTexture.h"
#include "render\\renderEffect.h"
#include "render\\renderMaterial.h"
#include "render\\renderVertices.h"
#include "render\\renderGeometry.h"
#include "render\\renderFog.h"
#include "render\\renderLight.h"
#include "render\\renderLightManager.h"
#include "render\\renderRenderTarget.h"
#include "render\\renderCamera.h"
#include "render\\renderCameraManager.h"
#include "render\\renderBinders.h"
#include "render\\renderManager.h"
#include "render\\renderLines3d.h"
#include "render\\renderLines2d.h"
#include "render\\renderMesh.h"
#include "render\\renderModel.h"
#include "render\\renderSprites.h"
#include "render\\renderGenerator.h"
#include "render\\renderAux.h"
#include "render\\renderFlameRenderer.h"
#include "render\\renderLensFlares.h"
#include "render\\renderFader.h"

#include "scene\\SceneTree.h"
//#include "scene\\sceneVisitor.h"
#include "scene\\scene.h"
#include "scene\\sceneManager.h"
#include "scene\\sceneBaseTrigger.h"
#include "scene\\sceneDistanceTrigger.h"

#include "ui/ui.h"
//#include "script\\script.h"