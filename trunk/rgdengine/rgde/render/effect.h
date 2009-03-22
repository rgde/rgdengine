#pragma once

#include <rgde/math/types3d.h>

namespace render
{
	typedef boost::shared_ptr<class texture> texture_ptr;

	typedef boost::shared_ptr<class Effect> PEffect;

	class Effect
	{
	public:
		struct Annotation
		{
			std::string name;
			std::string value;
		};
		typedef std::vector<Annotation> AnnotationsVector;

		class IParameter
		{
		public:
			enum EType
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

			virtual ~IParameter(){}

			virtual const std::string& get_name() const = 0;
			virtual const std::string& getSemantic() const = 0;
			virtual unsigned int getSize() const = 0;
			virtual EType getType() const = 0;

			virtual AnnotationsVector& getAnnotations() = 0;

			virtual bool set(const void* pData, unsigned int iSize) = 0;

			virtual bool set(int value) = 0;
			virtual bool set(bool value) = 0;
			virtual bool set(float value) = 0;
			virtual bool set(const std::string& value) = 0;
			virtual bool set(const math::Matrix33f& value) = 0;
			virtual bool set(const math::Matrix44f& value) = 0;
			virtual bool set(const math::Color& value) = 0;
			virtual bool set(const math::Vec4f& value) = 0;
			virtual bool set(const math::Vec3f& value) = 0;
			virtual bool set(const math::Vec2f& value) = 0;
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
			virtual bool set(const math::Matrix44f* value, int num) = 0;
			virtual bool set(const math::Vec4f* value, int num) = 0;
			virtual bool set(const math::Vec3f* value, int num) = 0;
			virtual bool set(const math::Vec2f* value, int num) = 0;
		};

		class ITechnique
		{
		public:
			class IPass
			{
			public:
				virtual ~IPass(){}

				virtual void begin() = 0;
				virtual void end() = 0;

				virtual const std::string& get_name() const = 0;
				virtual AnnotationsVector& getAnnotations() = 0;
			};

			virtual ~ITechnique(){}
			virtual std::vector <IPass*>& getPasses() = 0;
			virtual const std::string& get_name() const = 0;
			virtual AnnotationsVector& getAnnotations() = 0;
			
			virtual void begin() = 0;
			virtual void end() = 0;
		};

		typedef std::map <std::string, IParameter*> Parameters;
		typedef std::list <ITechnique*> Techniques;

		friend PEffect;
	protected:
		virtual void load(const std::string& fileName) = 0;
	public:
		virtual ~Effect(){}

		static PEffect create(const std::string& fileName);
		static void ClearAll();

		///////////////////////////////////////////////////

		virtual void commitChanges() = 0;
		virtual void onLostDevice() = 0;
		virtual void onResetDevice() = 0;
		virtual const std::string& get_name() const= 0;
		virtual Parameters& getParams() = 0;
		virtual Techniques& getTechnics() = 0;
		virtual ITechnique* findTechnique(const std::string& name) = 0;
	};
}