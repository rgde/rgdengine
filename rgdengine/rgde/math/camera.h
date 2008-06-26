#pragma once

#include <rgde/math/transform.h>
#include <rgde/math/frustum.h>

namespace math
{
	class BaseCamera : public Frame
	{
	public:
		BaseCamera() {}
		virtual ~BaseCamera(){}

		virtual void	setProjection(float fFovy, float fAspect, float fZNear, float fZFar);
		virtual void	activate();

		const Matrix44f  getViewMatrix()	const;
		const Matrix44f& getProjMatrix()	const {return m_ProjMatrix;}

		math::Frustum& getFrustum() { return m_frustum; }
		const math::Frustum& getFrustum() const { return m_frustum; }

		virtual void	 setPriority(unsigned priority) {}
		virtual unsigned getPriority() const {return 0;}

	protected:
		math::Matrix44f m_ProjMatrix;
		math::Frustum m_frustum;
	};

	typedef boost::intrusive_ptr<BaseCamera> PCamera;
}