#pragma once

#include <rgde/math/transform.h>
#include <rgde/render/manager.h>

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