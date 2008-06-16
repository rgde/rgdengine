#pragma once

#include <rgde/math/mathTransform.h>
#include <rgde/render/renderManager.h>

namespace render 
{
	class Bone : public math::Frame, public render::IRendererable 
	{
	public:
		Bone();

	protected: 		
		void render();
	};

	typedef boost::shared_ptr<Bone> PBone;
}