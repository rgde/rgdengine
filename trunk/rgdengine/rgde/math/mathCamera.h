#pragma once

#include <rgde/math/mathTransform.h>
#include <rgde/math/mathFrustum.h>

//Neonic: octree
#include <rgde/physic/collider.h>

namespace math
{
	class CBaseCamera
		: public CFrame
		//Neonic: octree
		, public collision::CCollisionObject
	{
	public:
		CBaseCamera() {}
		virtual ~CBaseCamera(){}

		virtual void	setProjection(float fFovy, float fAspect, float fZNear, float fZFar);
		virtual void	activate();

		const Matrix44f  getViewMatrix()	const;
		const Matrix44f& getProjMatrix()	const {return m_ProjMatrix;}

		math::CFrustum& getFrustum() { return m_frustum; }
		const math::CFrustum& getFrustum() const { return m_frustum; }

		virtual void	 setPriority(unsigned priority) {}
		virtual unsigned getPriority() const {return 0;}

	protected:
		math::Matrix44f m_ProjMatrix;
		math::CFrustum m_frustum;
	};

	typedef boost::intrusive_ptr<CBaseCamera> PCamera;
}