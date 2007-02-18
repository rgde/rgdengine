#include "precompiled.h"

#include "render/renderVertices.h"

#include <d3dx9.h>

namespace vertex
{
	PositionColored::PositionColored(const math::Vec3f &pos, math::Color c)
		: position(pos),
		  color(c)
	{
	}

	const VertexDecl PositionColored::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat3,	MethodDefault, UsagePosition,	0}, 
			{0,		12, TypeColor,	MethodDefault, UsageColor,		0}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const VertexDecl PositionColoredTextured::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat3,	MethodDefault, UsagePosition,	0}, 
			{0,		12, TypeColor,	MethodDefault, UsageColor,		0}, 
			{0,		16, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const VertexDecl PositionNormalColoredTexturedBinormalTangent::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,  0, TypeFloat3, MethodDefault, UsagePosition,	0}, 
			{0, 12, TypeFloat3, MethodDefault, UsageNormal,		0}, 
			{0, 24, TypeColor,  MethodDefault, UsageColor,		0}, 
			{0, 28, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{0, 36, TypeFloat3, MethodDefault, UsageBiNormal,	0}, 
			{0, 48, TypeFloat3, MethodDefault, UsageTangent,	0}, 
			{255,0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	//-----------------------------------------------------------------------------------
	const VertexDecl PositionColoredTextured2::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat3,	MethodDefault, UsagePosition,	0}, 
			{0,		12, TypeColor,	MethodDefault, UsageColor,		0}, 
			{0,		16, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{0,		24, TypeFloat2, MethodDefault, UsageTexCoord,	1}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const VertexDecl PositionNormal::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,  0, TypeFloat3, MethodDefault, UsagePosition,	0}, 
			{0, 12, TypeFloat3, MethodDefault, UsageNormal,		0}, 
			{255,0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const VertexDecl PositionNormalColored::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,  0, TypeFloat3, MethodDefault, UsagePosition,	0}, 
			{0, 12, TypeFloat3, MethodDefault, UsageNormal,		0}, 
			{0, 24, TypeColor,  MethodDefault, UsageColor,		0}, 
			{255,0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const VertexDecl PositionNormalColoredTextured::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,  0, TypeFloat3, MethodDefault, UsagePosition,	0}, 
			{0, 12, TypeFloat3, MethodDefault, UsageNormal,		0}, 
			{0, 24, TypeColor,  MethodDefault, UsageColor,		0}, 
			{0, 28, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{255,0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const VertexDecl PositionNormalColoredTextured2::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat3,	MethodDefault, UsagePosition,	0}, 
			{0,		12, TypeFloat3, MethodDefault, UsageNormal,		0}, 
			{0,		24, TypeColor,	MethodDefault, UsageColor,		0}, 
			{0,		28, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{0,		36, TypeFloat2, MethodDefault, UsageTexCoord,	1}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};
		return aDecl;
	}

	const VertexDecl Position::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat3,	MethodDefault, UsagePosition,	0}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};
		return aDecl;
	}

	const VertexDecl PositionTextured::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,  0, TypeFloat3, MethodDefault, UsagePosition,	0}, 
			{0, 12, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{255,0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}


	const VertexDecl PositionTextured2::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat3,	MethodDefault, UsagePosition,	0}, 
			{0,		12, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{0,		20, TypeFloat2, MethodDefault, UsageTexCoord,	1}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};
		return aDecl;
	}

	const VertexDecl PositionTransformed::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat4,	MethodDefault, UsagePositionT,	0}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};
		return aDecl;
	}

	PositionTransformedColored::PositionTransformedColored(const math::Vec4f &pos, math::Color c)
		: position(pos),
		  color(c)
	{
	}

	PositionTransformedColored::PositionTransformedColored(const math::Vec2f &pos, math::Color c)
		: position(pos[0], pos[1], 0, 0),
		  color(c)
	{
	}

	const VertexDecl PositionTransformedColored::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat4,	MethodDefault, UsagePositionT,	0}, 
			{0,		16, TypeColor,	MethodDefault, UsageColor,		0}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	//-----------------------------------------------------------------------------------
	const VertexDecl PositionTransformedColoredTextured::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat4,	MethodDefault, UsagePositionT,	0}, 
			{0,		16, TypeColor,	MethodDefault, UsageColor,		0}, 
			{0,		20, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const VertexDecl PositionNormalTexturedTangentBinorm::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,  0, TypeFloat3, MethodDefault, UsagePosition,	0}, 
			{0, 12, TypeFloat3, MethodDefault, UsageNormal,		0}, 
			{0, 24, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{0, 32, TypeFloat3, MethodDefault, UsageTangent,	0}, 
			{0, 44, TypeFloat3, MethodDefault, UsageBiNormal,	0}, 
			{255,0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const VertexDecl PositionNormalTextured2TangentBinorm::GetDecl()
	{
		static VertexElement aDecl[]=
		{
			{0,  0, TypeFloat3, MethodDefault, UsagePosition,	0}, 
			{0, 12, TypeFloat3, MethodDefault, UsageNormal,		0}, 
			{0, 24, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{0, 32, TypeFloat2, MethodDefault, UsageTexCoord,	1}, 
			{0, 40, TypeFloat3, MethodDefault, UsageTangent,	0}, 
			{0, 52, TypeFloat3, MethodDefault, UsageBiNormal,	0}, 
			{255,0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const vertex::VertexDecl PositionSkinnedNormalColoredTextured2TangentBinorm::GetDecl()
	{
		static vertex::VertexElement aDecl[] = {
			{0,  0, vertex::TypeFloat3, vertex::MethodDefault, vertex::UsagePosition,	  0},
			{0, 12, vertex::TypeFloat4, vertex::MethodDefault, vertex::UsageBlendWeight,  0},
			{0, 28, vertex::TypeUbyte4, vertex::MethodDefault, vertex::UsageBlendIndices, 0},
			{0, 32, vertex::TypeFloat3, vertex::MethodDefault, vertex::UsageNormal,		  0},
			{0, 44, vertex::TypeColor,  vertex::MethodDefault, vertex::UsageColor,        0},
			{0, 48, vertex::TypeFloat2, vertex::MethodDefault, vertex::UsageTexCoord,	  0},
			{0, 56, vertex::TypeFloat2, vertex::MethodDefault, vertex::UsageTexCoord,	  1},
			{0, 64, vertex::TypeFloat3, vertex::MethodDefault, vertex::UsageTangent,	  0},
			{0, 76, vertex::TypeFloat3, vertex::MethodDefault, vertex::UsageBiNormal,	  0},
			{255,0,	vertex::TypeUnused, (vertex::DeclMethod)0, (vertex::DeclUsage)0,	  0}
		};

		return aDecl;
	}
}