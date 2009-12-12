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