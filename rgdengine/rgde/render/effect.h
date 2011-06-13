#pragma once

#include <rgde/math/types3d.h>

namespace render
{
	typedef boost::shared_ptr<class texture> texture_ptr;

	typedef boost::shared_ptr<class effect> effect_ptr;

	class effect
	{
	public:
		struct annotation
		{
			std::string name;
			std::string value;
		};
		typedef std::vector<annotation> annotations_vector;

		class parameter
		{
		public:
			enum type_t
			{
				Void,
				Bool,
				Int,
				Float,
				String,
				Texture,
				Texture1D,
				Texture2D,
				Texture3D,
				TextureCube,
				Sampler,
				Sampler1D,
				Sampler2D,
				Sampler3D,
				SamplerCube,
				PixelShader,
				VertexShader,
				PixelFragment,
				VertexFragment
			};

			virtual ~parameter(){}

			virtual const std::string& name() const = 0;
			virtual const std::string& semantic() const = 0;
			virtual unsigned int size() const = 0;
			virtual type_t type() const = 0;

			virtual annotations_vector& get_annotations() = 0;

			virtual bool set(const void* data, unsigned int size) = 0;

			virtual bool set(int value) = 0;
			virtual bool set(bool value) = 0;
			virtual bool set(float value) = 0;
			virtual bool set(const std::string& value) = 0;
			virtual bool set(const math::Matrix33f& value) = 0;
			virtual bool set(const math::matrix44f& value) = 0;
			virtual bool set(const math::Color& value) = 0;
			virtual bool set(const math::vec4f& value) = 0;
			virtual bool set(const math::vec3f& value) = 0;
			virtual bool set(const math::vec2f& value) = 0;
			virtual bool set(const texture_ptr& value) = 0;

			template <class T>
			bool set(const T& val)
			{
				return set(&val, sizeof(T));
			}

			template <class T>
			bool set(const std::vector<T>& vec)
			{
				return set(&vec[0], sizeof(T)*(uint)(vec.size()));
			}

			virtual bool set(const int* value, int num) = 0;
			virtual bool set(const float* value, int num) = 0;
			virtual bool set(const math::Matrix33f* value, int num) = 0;
			virtual bool set(const math::matrix44f* value, int num) = 0;
			virtual bool set(const math::vec4f* value, int num) = 0;
			virtual bool set(const math::vec3f* value, int num) = 0;
			virtual bool set(const math::vec2f* value, int num) = 0;
		};

		class technique
		{
		public:
			class pass
			{
			public:
				virtual ~pass(){}

				virtual void begin() = 0;
				virtual void end() = 0;

				virtual const std::string& name() const = 0;
				virtual annotations_vector& get_annotations() = 0;
			};

			typedef std::vector <pass*> passes;

			virtual ~technique(){}
			virtual passes& get_passes() = 0;
			virtual const std::string& name() const = 0;
			virtual annotations_vector& get_annotations() = 0;
			
			virtual void begin() = 0;
			virtual void end() = 0;
		};

		typedef std::map <std::string, parameter*> params_map;
		typedef std::list <technique*> techniques_list;

		friend effect_ptr;
	protected:
		virtual bool load(const std::string& filename) = 0;
	public:
		virtual ~effect(){}

		static effect_ptr create(const std::string& filename);
		static void clear_all();

		///////////////////////////////////////////////////

		virtual void commit_changes() = 0;
		virtual void onLostDevice() = 0;
		virtual void onResetDevice() = 0;
		virtual const std::string& name() const= 0;
		virtual params_map& get_params() = 0;
		virtual techniques_list& get_technics() = 0;
		virtual technique* find_technique(const std::string& name) = 0;
	};
}