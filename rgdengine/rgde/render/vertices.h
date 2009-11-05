#pragma once

#include <rgde/math/camera.h>

namespace  vertex
{
	// Vertex element semantics
	//
	enum DeclUsage
	{
		UsagePosition	= 0,
		UsageBlendWeight,   // 1
		UsageBlendIndices,  // 2
		UsageNormal,        // 3
		UsagePSize,         // 4
		UsageTexCoord,      // 5
		UsageTangent,       // 6
		UsageBiNormal,      // 7
		UsageTessFactor,    // 8
		UsagePositionT,     // 9
		UsageColor,         // 10
		UsageFog,           // 11
		UsageDepth,         // 12
		UsageSample,        // 13
	};

	enum DeclMethod
	{
		MethodDefault = 0,
		MethodPartialU,
		MethodPartialV,
		MethodCrossUV,				// Normal
		MethodUV,
		MethodLookup,               // Lookup a displacement map
		MethodLookupPresampled,     // Lookup a pre-sampled displacement map
	};


	// Declarations for _Type fields
	enum Type
	{
		TypeFloat1  =  0,   // 1D float expanded to (value, 0., 0., 1.)
		TypeFloat2  =  1,   // 2D float expanded to (value, value, 0., 1.)
		TypeFloat3  =  2,   // 3D float expanded to (value, value, value, 1.)
		TypeFloat4  =  3,   // 4D float
		TypeColor	=  4,   // 4D packed unsigned bytes mapped to 0. to 1. range
		// Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
		TypeUbyte4  =  5,   // 4D unsigned byte
		TypeShort2  =  6,   // 2D signed short expanded to (value, value, 0., 1.)
		TypeShort4  =  7,   // 4D signed short

		// The following types are valid only with vertex shaders >= 2.0

		TypeUbyte4n   =  8, // Each of 4 bytes is normalized by dividing to 255.0
		TypeShort2n   =  9, // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
		TypeShort4n   = 10, // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
		TypeUshort2n  = 11, // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
		TypeUshort4n  = 12, // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
		TypeUdec3     = 13, // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
		TypeDec3n     = 14, // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
		TypeFloat16_2 = 15, // Two 16-bit floating point values, expanded to (value, value, 0, 1)
		TypeFloat6_4  = 16, // Four 16-bit floating point values
		TypeUnused    = 17  // When the type field in a decl is unused.
	};

	struct VertexElement
	{
		unsigned short  usStream;     // Stream index
		unsigned short  usOffset;     // Offset in the stream in bytes
		unsigned char	eType;        // Data type
		unsigned char	eMethod;      // Processing method
		unsigned char	eUsage;       // Semantics
		unsigned char   ucUsageIndex; // Semantic index
	};

	typedef  VertexElement* VertexDecl;

	/// шаблон-требование дл€ структуры определ€ющей вершину
	template <class T>
	struct TCustomVertex
	{
		size_t GetSize(){return sizeof(T);}
	};

	struct PositionNormalColoredTexturedBinormalTangent
	{
		PositionNormalColoredTexturedBinormalTangent(){}
		PositionNormalColoredTexturedBinormalTangent(const math::vec3f& pos, const math::vec3f& n, const math::Color& c, 
			const math::vec2f& tv, const math::vec3f& bn, const math::vec3f& tan)
			: position(pos), normal(n), color(c), tex(tv), binormal(bn), 
			tangent(tan)
		{}

		math::vec3f position;
		math::vec3f normal;
		math::Color	color;
		math::vec2f tex;
		math::vec3f binormal;
		math::vec3f tangent;

		static const VertexDecl get_decl();
	};

	//typedef PositionNormalColoredTexturedBinormalTangent MeshVertex;

	/// позици€ + цвет
	struct position_colored : public TCustomVertex<position_colored>
	{
		position_colored(){}
		position_colored(const math::vec3f& pos, math::Color color);

		math::vec3f position;
		math::Color	color;
		static const VertexDecl get_decl();
	};

	/// позици€ + цвет + текстура
	struct PositionColoredTextured : public TCustomVertex<PositionColoredTextured>
	{
		math::vec3f		position;
		math::Color		color;
		math::vec2f		tex;
		static const VertexDecl get_decl();
	};

	/// позици€ + цвет + текстура1 + текстура2
	struct PositionColoredTextured2 : public TCustomVertex<PositionColoredTextured2>
	{
		PositionColoredTextured2() {}
		PositionColoredTextured2( const math::vec3f& pos, const math::Color c,
			const math::vec2f& tex0In, const math::vec2f& tex1In ) 
			: position(pos), color(c), tex0(tex0In), tex1(tex1In)
		{}

		math::vec3f		position;
		math::Color		color;
		math::vec2f		tex0;
		math::vec2f		tex1;

		static const VertexDecl get_decl();
	};

	struct PositionNormal : public TCustomVertex<PositionNormal>
	{
		math::vec3f		position;
		math::vec3f		normal;
		static const VertexDecl get_decl();
	};

	struct PositionNormalColored : public TCustomVertex<PositionNormalColored>
	{
		math::vec3f		position;
		math::vec3f		normal;
		math::Color		color;

		static const VertexDecl get_decl();
	};

	struct PositionNormalColoredTextured : public TCustomVertex<PositionNormalColoredTextured>
	{
		math::vec3f		position;
		math::vec3f		normal;
		math::Color		color;
		math::vec2f		tex;

		static const VertexDecl get_decl();
	};

	struct PositionNormalColoredTextured2 : public TCustomVertex<PositionNormalColoredTextured2>
	{
		math::vec3f		position;
		math::vec3f		normal;
		math::Color		color;
		math::vec2f		tex0;
		math::vec2f		tex1;

		static const VertexDecl get_decl();
	};

	struct Position : public TCustomVertex<Position>
	{
		math::vec3f		position;

		static const VertexDecl get_decl();
	};

	struct PositionTextured : public TCustomVertex<PositionTextured>
	{
		math::vec3f		position;
		math::vec2f		tex;

		static const VertexDecl get_decl();
	};

	struct PositionTextured2 : public TCustomVertex<PositionTextured2>
	{
		math::vec3f		position;
		math::vec2f		tex0;
		math::vec2f		tex1;

		static const VertexDecl get_decl();
	};


	struct PositionTransformed : public TCustomVertex<PositionTransformed>
	{
		math::vec4f		position;
		static const VertexDecl get_decl();
	};

	struct position_transformed_colored : public TCustomVertex<position_transformed_colored>
	{
		position_transformed_colored(){}
		position_transformed_colored(const math::vec4f& pos, math::Color color);
		position_transformed_colored(const math::vec2f& pos, math::Color color);

		math::vec4f		position;
		math::Color		color;
		static const VertexDecl get_decl();
	};

	struct PositionTransformedColoredTextured : public TCustomVertex<PositionTransformedColoredTextured>
	{
		PositionTransformedColoredTextured() {}
		PositionTransformedColoredTextured( const math::vec4f& pos, math::Color c, 
			const math::vec2f& texIn ) : position(pos), color(c), tex(texIn)
		{}

		math::vec4f		position;
		math::Color		color;
		math::vec2f		tex;

		static const VertexDecl get_decl();
	};

	struct PositionNormalTexturedTangentBinorm : public TCustomVertex<PositionNormalTexturedTangentBinorm>
	{
		math::vec3f		position;
		math::vec3f		normal;
		math::vec2f		tex;
		math::vec3f		tangent;
		math::vec3f		binormal;
		static const VertexDecl get_decl();
	};

	struct PositionNormalTextured2TangentBinorm : public TCustomVertex<PositionNormalTextured2TangentBinorm>
	{
		math::vec3f		position;
		math::vec3f		normal;
		math::vec2f		tex0;
		math::vec2f		tex1;
		math::vec3f		tangent;
		math::vec3f		binormal;
		static const VertexDecl get_decl();
	};

	struct PositionSkinnedNormalColoredTextured2TangentBinorm : public vertex::TCustomVertex<PositionSkinnedNormalColoredTextured2TangentBinorm>
	{
		math::vec3f		position;
		math::vec4f		weights;
		unsigned char   indices[4];
		math::vec3f		normal;
		math::Color	    color;
		math::vec2f		tex0;
		math::vec2f		tex1;
		math::vec3f		tangent;
		math::vec3f		binormal;

		static const vertex::VertexDecl get_decl();
	};

	typedef PositionNormalTextured2TangentBinorm MeshVertex;
}