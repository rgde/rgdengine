#pragma once

#include <rgde/math/transform.h>
#include <rgde/math/frustum.h>

namespace math
{
	class base_camera : public frame
	{
	public:
		base_camera();
		~base_camera();

		virtual void	projection(float fovy, float aspect, float z_near, float z_far);
		virtual void	activate();

		const matrix44f  view_matrix()	const;
		const matrix44f& proj_matrix()	const {return m_projection;}

		math::frustum& frustum() { return m_frustum; }
		const math::frustum& frustum() const { return m_frustum; }

		virtual void	 priority(unsigned p) {}
		virtual unsigned priority() const {return 0;}

	protected:
		math::matrix44f m_projection;
		math::frustum m_frustum;
	};

	typedef boost::intrusive_ptr<base_camera> camera_ptr;
}