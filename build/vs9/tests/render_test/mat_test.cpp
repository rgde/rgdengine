class material
{

	material_instance_ptr make_instance();
};

class material_instance
{
	void set_base_material();

	struct 
};



namespace resources
{
	struct buff
	{
		size_t num_bytes;
		void* data;
	};

	struct model
	{
	}

	struct mesh
	{
	};

	struct material
	{
		buff name;
		buff shader_name;
	};

	struct material_instance
	{
		struct param
		{
			buff name;

			union
			{
				float float_value;
				math::vec2f vec2_value;
				math::vec3f vec3_value;
				math::vec4f vec4_value;
				math::color color_value;
			};
		};

		buff base_mat_name; // "default" by default :)

		size_t m_params_num;
		param* m_params_array;
	};

}