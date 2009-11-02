#pragma once

namespace math
{
	class frame;
}

namespace render
{
	class mesh;
}

namespace scene
{
	class base_visitor //: public base::TVisitor<math::frame>, 
						//  public base::TVisitor<render::mesh>
	{
	public:
		virtual bool visit(math::frame* t) = 0;
		virtual bool visit(render::mesh* t) = 0;
	};
}