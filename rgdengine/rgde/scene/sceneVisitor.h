#pragma once

namespace math
{
	class Frame;
}

namespace render
{
	class Mesh;
}

namespace scene
{

	class ISceneVisitor //: public base::TVisitor<math::Frame>, 
						//  public base::TVisitor<render::Mesh>
	{
	public:
		virtual bool visit(math::Frame* t) = 0;
		virtual bool visit(render::Mesh* t) = 0;
	};
}