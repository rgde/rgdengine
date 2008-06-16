#pragma once

namespace math
{
	class CFrame;
}

namespace render
{
	class Mesh;
}

namespace scene
{

	class ISceneVisitor //: public base::TVisitor<math::CFrame>, 
						//  public base::TVisitor<render::Mesh>
	{
	public:
		virtual bool visit(math::CFrame* t) = 0;
		virtual bool visit(render::Mesh* t) = 0;
	};
}