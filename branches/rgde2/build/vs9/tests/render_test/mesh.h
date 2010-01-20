#pragma once

namespace rgde
{
	namespace render
	{
		struct material
		{

		};

		typedef boost::shared_ptr<material> material_ptr;

		typedef boost::shared_ptr<class mesh> mesh_ptr;

		class mesh
		{
		public:
			struct attrib_range
			{
				size_t id;
				size_t index_start;
				size_t prim_count;
				size_t vertex_start;
				size_t vertex_count;
				primitive_type prim_type;
			};

			static mesh_ptr create_box(device& dev, float x, float y, float z);
			//static mesh_ptr create_sphere(float r);
			//static mesh_ptr create_conus(float h, float r);
			//static mesh_ptr create_cylinder(float h, float r);

			mesh(device& dev) : m_device(dev){}

			void render();
			void render(size_t arrib_index);

			vertex_buffer_ptr vb;
			index_buffer_ptr ib;
			std::vector<std::pair<attrib_range, material_ptr>> m_parts;

			//TODO: custom atribs, bbox, bsphere, bones, etc
			device& get_device() {return m_device;}
			const device& get_device() const {return m_device;}

		protected:
			device& m_device;
		};
	}
}
//
//enum primitive_type
//{
//	point_list         = 1,
//	line_list          = 2,
//	line_strip         = 3,
//	triangle_list      = 4,
//	triangle_strip     = 5,
//	triangle_fan       = 6,
//	pt_max_value       = 0x7fffffff, /* force 32-bit size enum */
//};
//
//struct mesh
//{
//	struct sub_mesh
//	{
//		uint id;
//
//		uint vb_start;
//		uint vb_end;
//		uint ib_start;
//		uint ib_end;
//
//		uint vert_size;
//
//		uint prim_count;
//		uint vertex_count;
//
//		primitive_type prim_type;
//
//		decl_ptr	decl;
//		material_ptr mat;
//
//		std::string name;		
//	}
//
//	typedef std::vector<sub_mesh> sub_meshes;
//	typedef std::vector<material_ptr> materials;
//
//	sub_meshes m_sub_meshes;
//	materials m_materials;
//	vb_ptr m_vb;
//	ib_ptr m_id;
//
//	std::vector<byte> m_raw_vb_data;
//	std::vector<byte> m_raw_ib_data;
//
//	aabb m_bbox;
//	//bones:
//	std::vector<math::mat44f> m_invtr_bones;
//	std::vector<std::string> m_bones_names;
//};

//struct col_mesh
//{
//	struct col_vertex
//	{
//		float x, y, z;
//		float nx, ny, nz;
//	};
//
//	struct part
//	{
//		uint begin, end;
//		std::string name;
//	};
//
//	std::vector<col_vertex> m_vertexies;
//	std::vector<part> m_parts;
//	aabb m_bbox;
//};
//
//struct model
//{
//	enum {max_lodes = 3};
//	mesh_ptr m_lodes[max_lodes];
//	float m_lod_distance[max_lodes];
//
//	col_mesh m_col_mesh;
//	aabb m_bbox;
//	bsphere m_bbsphere;
//};