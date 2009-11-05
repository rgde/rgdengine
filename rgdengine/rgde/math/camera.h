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

		virtual void	set_projection(float fovy, float aspect, float z_near, float z_far);
		virtual void	activate();

		const matrix44f  get_view_matrix()	const;
		const matrix44f& get_proj_matrix()	const {return m_projection;}

		math::frustum& get_frustum() { return m_frustum; }
		const math::frustum& get_frustum() const { return m_frustum; }

		virtual void	 set_priority(unsigned priority) {}
		virtual unsigned get_priority() const {return 0;}

	protected:
		math::matrix44f m_projection;
		math::frustum m_frustum;
	};

	typedef boost::intrusive_ptr<base_camera> camera_ptr;
}