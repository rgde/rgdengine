#include "stdafx.h"

#include "camera.h"

namespace rgde
{
namespace scene
{
	camera::camera()
	{
	}

	camera::~camera()
	{
	}

	const math::mat44f  camera::get_view_matrix()	const 
	{
		math::mat44f m = frame::get_full_tm();
		math::invert(m);
		return m;		
	}

	void camera::set_projection(float fovy, float aspect, float zn, float zf)
	{
		float hfov = fovy/2;
		float yScale = ::cos(hfov)/::sin(hfov);
		float xScale = yScale / aspect;

		m_projection.set(xScale,	0,			0,			0,
							0,		yScale,		0,			0, 
							0,		0,		zf/(zf-zn),		1,
							0,		0,		-zn*zf/(zf-zn), 0);

		math::transpose(m_projection);
	}

	void camera::activate()
	{
		m_frustum.calculate(get_view_matrix(), get_proj_matrix());
	}
}
}