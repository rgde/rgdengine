#include "stdafx.h"

#include "mesh.h"

namespace rgde
{
	namespace render
	{
		typedef unsigned short uint16;

		namespace
		{
			static inline void sincosf( float angle, float* psin, float* pcos )
			{
				*psin = sinf( angle );
				*pcos = cosf( angle );
			}

			using rgde::render::vertex_element;
			vertex_element vertex_desc[] = 
			{
				{0, 0,  vertex_element::float3,   vertex_element::default_method, vertex_element::position, 0}, 
				{0, 12, vertex_element::float3,   vertex_element::default_method, vertex_element::normal,	0},
				{0, 24, vertex_element::float2,   vertex_element::default_method, vertex_element::texcoord,	0},
				vertex_element::end_element
			};

			struct vertex
			{
				math::vec3f pos;
				math::vec3f norm;
				math::vec2f uv;
			};

			mesh_ptr create_mesh(device& dev, vertex* vertices, size_t num_vertices, uint16* indices, size_t num_indices)
			{
				mesh_ptr out(new mesh(dev));

				vertex_declaration_ptr decl = vertex_declaration::create(dev, vertex_desc, 4);

				{
					out->vb = vertex_buffer::create
						(
						dev, decl, 
						num_vertices * sizeof(vertex), 
						resource::default, 
						buffer::write_only
						);

					void *vb = out->vb->lock( 0, num_vertices * sizeof(vertex), 0 );
					memcpy( vb, vertices, num_vertices * sizeof(vertex));
					out->vb->unlock();
				}

				{
					out->ib = index_buffer::create(dev, num_indices*sizeof(uint16), false, resource::default, buffer::write_only);
					void *ib = out->ib->lock( 0, num_indices*sizeof(uint16), 0 );
					memcpy( ib, indices, num_indices*sizeof(uint16) );
					out->ib->unlock();
				}

				out->m_parts.resize(1);

				mesh::attrib_range& attr = out->m_parts[0].first;
				attr.id = 0;
				attr.index_start = 0;
				attr.prim_count = num_indices/3;
				attr.vertex_start = 0;
				attr.vertex_count = num_vertices;
				attr.prim_type = triangle_list;

				return out;
			}
		}


		void mesh::render()
		{
			m_device.set_stream_source( 0, vb, sizeof(vertex) );
			m_device.set_index_buffer(ib);

			for (size_t i = 0, size = m_parts.size(); i < size; ++i)
			{
				attrib_range& attr = m_parts[i].first;

				m_device.draw(attr.prim_type, attr.vertex_start, 0, attr.vertex_count,
					attr.index_start, attr.prim_count);
			}
		}

		void mesh::render(size_t attrib_index)
		{
			m_device.set_stream_source( 0, vb, sizeof(vertex) );
			m_device.set_index_buffer(ib);

			attrib_range& attr = m_parts[attrib_index].first;
			m_device.draw(attr.prim_type, attr.vertex_start, 0, attr.vertex_count,
				attr.index_start, attr.prim_count);
		}

		static float cubeN[6][3] =
		{
			{-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
			{0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}
		};

		static WORD cubeF[6][4] =
		{
			{ 0, 1, 5, 4 }, { 4, 5, 6, 7 }, { 7, 6, 2, 3 },
			{ 1, 0, 3, 2 }, { 1, 2, 6, 5 }, { 0, 4, 7, 3 }
		};

		static float cubeV[8][3] =
		{
			// Lower tier (lower in y)
			{-.5f, -.5f, -.5f},
			{-.5f, -.5f,  .5f},
			{ .5f, -.5f,  .5f},
			{ .5f, -.5f, -.5f},

			// Upper tier
			{-.5f, .5f, -.5f},
			{-.5f, .5f,  .5f},
			{ .5f, .5f,  .5f},
			{ .5f, .5f, -.5f},
		};

		static float cubeT[4][2] =
		{
			// Lower tier (lower in y)
			{0.0f, 0.0f},
			{0.0f, 1.0f},
			{1.0f, 1.0f},
			{1.0f, 0.0f}
		};

		static WORD cubeFT[6][4] =
		{
			{ 3, 0, 1, 2 }, { 0, 1, 2, 3 }, { 1, 2, 3, 0 },
			{ 0, 1, 2, 3 }, { 3, 0, 1, 2 }, { 0, 1, 2, 3 }
		};

		mesh_ptr mesh::create_box(device& dev, float x, float y, float z)
		{
			// Create the mesh
			const size_t num_faces = 12;
			const size_t num_vertices = 24;

			boost::scoped_array<vertex> vb (new vertex[num_vertices]);
			boost::scoped_array<uint16> ib (new uint16[num_faces*3]);

				// Fill in the data
				vertex* pv = vb.get();
				uint16* pf = ib.get();
				uint vertex_index = 0;

			// i iterates over the faces, 2 triangles per face
			for( int i = 0; i < 6; i++ )
			{
				for( int j = 0; j < 4; j++ )
				{
					pv->pos[0] = cubeV[cubeF[i][j]][0] * x;
					pv->pos[1] = cubeV[cubeF[i][j]][1] * y;
					pv->pos[2] = cubeV[cubeF[i][j]][2] * z;

					pv->norm[0] = cubeN[i][0];
					pv->norm[1] = cubeN[i][1];
					pv->norm[2] = cubeN[i][2];

					pv->uv[0] = cubeT[cubeFT[i][j]][0];
					pv->uv[1] = cubeT[cubeFT[i][j]][1];

					pv++;
				}

				pf[0] = ( uint16 )( vertex_index );
				pf[1] = ( uint16 )( vertex_index + 1 );
				pf[2] = ( uint16 )( vertex_index + 2 );
				pf += 3;

				pf[0] = ( uint16 )( vertex_index + 2 );
				pf[1] = ( uint16 )( vertex_index + 3 );
				pf[2] = ( uint16 )( vertex_index );
				pf += 3;

				vertex_index += 4;
			}

			mesh_ptr out = create_mesh(dev, vb.get(), num_vertices, ib.get(), num_faces*3);

			return out;
		}
	}
}