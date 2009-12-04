#pragma once

#include "transform.h"
#include "frustum.h"

namespace rgde
{
namespace scene
{
	class camera : public frame
	{
	public:
		camera();
		~camera();

		virtual void	set_projection(float fovy, float aspect, float z_near, float z_far);
		virtual void	activate();

		const math::mat44f  get_view_matrix()	const;
		const math::mat44f& get_proj_matrix()	const {return m_projection;}

		math::frustum& get_frustum() { return m_frustum; }
		const math::frustum& get_frustum() const { return m_frustum; }

		virtual void	 set_priority(unsigned priority) {}
		virtual unsigned get_priority() const {return 0;}

	protected:
		math::mat44f m_projection;
		math::frustum m_frustum;
	};

	typedef boost::intrusive_ptr<camera> camera_ptr;
}
}