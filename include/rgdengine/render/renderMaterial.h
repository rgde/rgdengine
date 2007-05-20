#pragma once

#include "core/NamedObject.h"
#include "math/mathTypes3d.h"
#include "render/renderTexture.h"
#include "render/renderEffect.h"
#include "render/renderBinder.h"

namespace render
{
	typedef boost::shared_ptr<class CMaterial> PMaterial;

	class CMaterial : public core::NamedObject//, game::IDynamicObject
	{
		CMaterial(math::Color amb, math::Color diff, math::Color spec,
				  math::Color em, float power);

		void load(const std::string& fileName);

	public:
		class CMaterialMap
		{
		public:
			enum EDefaultTexture
			{
				Black            = 1,
				White            = 2,
				DefaultNormalMap = 3
			};

			CMaterialMap(EDefaultTexture defaultTexture = White);

			const PTexture& getTexture() const { return m_pTexture; }
			void setTexture(const PTexture& texure);

			//Returns true if pTexture was valid for the last call of setTexture
			bool isTextureValid() const { return m_bTextureIsValid; }

			const math::Matrix44f& getMatrix() const { return m_matrix; }
			math::Matrix44f&       getMatrix()       { return m_matrix; }

			const math::Vec3f& getScrollSpeed() const { return m_scrollSpeed; }
			math::Vec3f&       getScrollSpeed()       { return m_scrollSpeed; }

			float	getRotationSpeed() const			{ return m_fRotationSpeed; }
			void	setRotationSpeed(float f)			{ m_fRotationSpeed = f;}

			const math::Vec3f&	getRotationCenter() const { return m_rotationCenter; }
			void				setRotationCenter(const math::Vec3f& v)       {m_rotationCenter = v; }

			void update(float fDt);

		private:

			PTexture        m_pDefaultTexture;

			bool            m_bTextureIsValid;

			PTexture        m_pTexture;
			math::Matrix44f m_matrix;
			math::Vec3f     m_scrollSpeed;
			float           m_fRotationSpeed;
			math::Vec3f     m_rotationCenter;

			float m_fTime;
		};

		typedef std::map<std::string, CMaterialMap> MaterialMaps;

		static PMaterial Create(math::Color amb = math::Black,
				  math::Color diff = math::White,
				  math::Color spec = math::Black,
				  math::Color em = math::Black,
				  float power = 1.0f);

		static PMaterial Create(const std::string& fileName);

		virtual ~CMaterial();

		bool isTransparent() const;

		const math::Color& getDiffuseColor()	const {return m_diffuse;}
		const math::Color& getAmbientColor()	const {return m_ambient;}
		const math::Color& getSpecularColor()	const {return m_specular;}
		const math::Color& getEmissiveColor()	const {return m_emissive;}

		void setDiffuseColor(const math::Color& c)	{m_diffuse  = c;}
		void setAmbientColor(const math::Color& c)	{m_ambient  = c;}
		void setSpecularColor(const math::Color& c) {m_specular = c;}
		void setEmissiveColor(const math::Color& c) {m_emissive = c;}

		float getSpecularPower()		const	{return m_fPower;}
		void setSpecularPower(float p)			{m_fPower = p;}		

		CMaterialMap&		getMaterialMap( const std::string& type);
		const CMaterialMap& getMaterialMap( const std::string& type) const;

		const PTexture&		getTextureMap( const std::string& type) const;

		MaterialMaps&		getMaterialMaps()		{ return m_maps;}
		const MaterialMaps& getMaterialMaps() const { return m_maps;}
		
		void update(float fDt);

		void  setEffect(const PEffect& pEffect);
		const PDynamicBinder& getDynamicBinder();
		IEffect::ITechnique* getTechnique() const;

	protected:		
		PDynamicBinder	m_pDynamicBinder;
		IEffect::ITechnique* m_technique;
		MaterialMaps	m_maps;
		math::Color     m_diffuse;        // Diffuse color RGBA 
		math::Color     m_ambient;        // Ambient color RGB 
		math::Color     m_specular;       // Specular 'shininess' 
		math::Color     m_emissive;       // Emissive color RGB 
		float           m_fPower;         // Sharpness if specular highlight

		std::string		m_file_name;
	};
}