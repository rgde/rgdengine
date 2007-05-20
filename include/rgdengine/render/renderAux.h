#pragma once

#include "math/mathTransform.h"
#include "render/renderManager.h"

namespace render 
{
	class CBone : public math::CFrame, public render::IRendererable 
	{
	public:
		CBone();

	protected: 		
		void render();
	};

	typedef boost::shared_ptr<CBone> PBone;
}