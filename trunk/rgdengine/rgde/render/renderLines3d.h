#pragma once

#include <rgde/render/renderGeometry.h>
#include <rgde/render/renderManager.h>

namespace render
{
	class CLine3dManager : public IRendererable
	{
	protected:
		CLine3dManager( unsigned long uPriority = 10);
		~CLine3dManager() {};

	public:
		typedef vertex::PositionColored Point;

		void addLine( const math::Vec3f& vPoint1, const math::Vec3f& vPoint2, 
			const math::Color& color = 0xffffffff );
		void addBox( const math::Matrix44f& m, const math::Vec3f& size,
			const math::Color& color = 0xffffffff );

		void addBox(const math::Matrix44f& m, const math::AABoxf& box,
			const math::Color& color = 0xffffffff );

		void addBox(const math::Vec3f& size, const math::Color& color = 0xffffffff );
		void addBox(const math::AABoxf& box, const math::Color& color = 0xffffffff );

		void addDirection( const math::Matrix44f& m, const math::Point3f& dir,
			const math::Color& color = 0xffffffff );
		void addSphere(const math::Matrix44f& m, float rad, int angle );
		void addQuad (const math::Vec3f& center, const math::Vec2f& size, float fSpin);

	protected:
		void render();

	protected:
		PEffect			m_spEffect;
		unsigned long	m_uPriority;			///> Приоритет менеджера линий

		typedef TGeometry<vertex::PositionColored> Geometry;
		Geometry m_Geometry;					///> Геометрия
		std::vector<vertex::PositionColored>* m_pVertices;
	};

	typedef base::TSingelton<CLine3dManager> Line3dManager;
} //~ namespace utility