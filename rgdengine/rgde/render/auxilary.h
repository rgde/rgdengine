#pragma once

#include <rgde/math/transform.h>
#include <rgde/render/manager.h>

namespace render 
{
	class Bone : public math::frame, public render::rendererable 
	{
	public:
		Bone();

	protected: 		
		void render();
	};

	typedef boost::shared_ptr<Bone> PBone;
}