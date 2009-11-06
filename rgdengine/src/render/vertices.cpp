#include "precompiled.h"

#include <rgde/render/vertices.h>

#include <d3dx9.h>

namespace vertex
{
	position_colored::position_colored(const math::vec3f &pos, math::Color c)
		: position(pos),
		  color(c)
	{
	}

	const vertex_decl position_colored::get_decl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat3,	MethodDefault, UsagePosition,	0}, 
			{0,		12, TypeColor,	MethodDefault, UsageColor,		0}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const vertex_decl PositionColoredTextured::get_decl()
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

	const vertex_decl PositionNormalColoredTexturedBinormalTangent::get_decl()
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
	const vertex_decl PositionColoredTextured2::get_decl()
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

	const vertex_decl PositionNormal::get_decl()
	{
		static VertexElement aDecl[]=
		{
			{0,  0, TypeFloat3, MethodDefault, UsagePosition,	0}, 
			{0, 12, TypeFloat3, MethodDefault, UsageNormal,		0}, 
			{255,0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}

	const vertex_decl PositionNormalColored::get_decl()
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

	const vertex_decl PositionNormalColoredTextured::get_decl()
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

	const vertex_decl PositionNormalColoredTextured2::get_decl()
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

	const vertex_decl Position::get_decl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat3,	MethodDefault, UsagePosition,	0}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};
		return aDecl;
	}

	const vertex_decl PositionTextured::get_decl()
	{
		static VertexElement aDecl[]=
		{
			{0,  0, TypeFloat3, MethodDefault, UsagePosition,	0}, 
			{0, 12, TypeFloat2, MethodDefault, UsageTexCoord,	0}, 
			{255,0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};

		return aDecl;
	}


	const vertex_decl PositionTextured2::get_decl()
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

	const vertex_decl PositionTransformed::get_decl()
	{
		static VertexElement aDecl[]=
		{
			{0,		0,  TypeFloat4,	MethodDefault, UsagePositionT,	0}, 
			{0xFF,	0,	TypeUnused, (DeclMethod)0, (DeclUsage)0,	0}
		};
		return aDecl;
	}

	position_transformed_colored::position_transformed_colored(const math::vec4f &pos, math::Color c)
		: position(pos),
		  color(c)
	{
	}

	position_transformed_colored::position_transformed_colored(const math::vec2f &pos, math::Color c)
		: position(pos[0], pos[1], 0, 0),
		  color(c)
	{
	}

	const vertex_decl position_transformed_colored::get_decl()
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
	const vertex_decl PositionTransformedColoredTextured::get_decl()
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

	const vertex_decl PositionNormalTexturedTangentBinorm::get_decl()
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

	const vertex_decl PositionNormalTextured2TangentBinorm::get_decl()
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

	const vertex::vertex_decl PositionSkinnedNormalColoredTextured2TangentBinorm::get_decl()
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