#pragma once

#include <rgde/math/transform.h>
#include <rgde/math/frustum.h>

namespace math
{
	class base_camera : public frame
	{
	public:
		base_camera() {}
		virtual ~base_camera(){}

		virtual void	setProjection(float fFovy, float fAspect, float fZNear, float fZFar);
		virtual void	activate();

		const Matrix44f  getViewMatrix()	const;
		const Matrix44f& getProjMatrix()	const {return m_ProjMatrix;}

		math::frustum& getFrustum() { return m_frustum; }
		const math::frustum& getFrustum() const { return m_frustum; }

		virtual void	 setPriority(unsigned priority) {}
		virtual unsigned get_priority() const {return 0;}

	protected:
		math::Matrix44f m_ProjMatrix;
		math::frustum m_frustum;
	};

	typedef boost::intrusive_ptr<base_camera> camera_ptr;
}