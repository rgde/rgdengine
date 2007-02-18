#pragma once

namespace math
{
	class CFrame;
}

namespace render
{
	class CMesh;
}

namespace scene
{

	class ISceneVisitor //: public base::TVisitor<math::CFrame>, 
						//  public base::TVisitor<render::CMesh>
	{
	public:
		virtual bool visit(math::CFrame* t) = 0;
		virtual bool visit(render::CMesh* t) = 0;
	};
}