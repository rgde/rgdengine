#pragma once

#include <rgde/math/mathTransform.h>
#include <rgde/render/renderManager.h>

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