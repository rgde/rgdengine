#include "stdafx.h"

#include "camera.h"

namespace rgde
{
	using namespace math;
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

	//////////////////////////////////////////////////////////////////////////

	free_camera::free_camera(camera_ptr camera)
		: base_camera_controller(camera)
		, m_up(0.0f, 1.0f, 0.0f)
		, m_eye_pos(0.0f, 0.0f, 0.0f)
		, m_lookat_pt(0.0f, 0.0f, 1.0f)
		, m_fixed_up(true)
	{
		//base::lmsg << "free_camera::free_camera()";
		if (camera)
		{
			m_up = camera->getUpGlobal();
			m_eye_pos = camera->get_world_pos();
			m_lookat_pt = camera->getAtGlobal();
		}

		do_ortho_normal();
	}

	free_camera_ptr free_camera::create(camera_ptr camera)
	{
		return free_camera_ptr(new free_camera(camera));
	}

	void free_camera::set_position(const vec3f& up, const vec3f& eye, const vec3f& look_at)
	{
		m_up       = up;
		m_eye_pos    = eye;
		m_lookat_pt = look_at;
		apply();
	}

	void free_camera::get_pos(vec3f& up, vec3f& eye, vec3f& look_at)
	{
		up       = m_up;
		eye    = m_eye_pos;
		look_at = m_lookat_pt;
	}

	void free_camera::move_forward(float d)
	{
		vec3f delta = m_lookat_pt-m_eye_pos;
		normalize(delta);
		delta *= d;

		m_eye_pos    += delta;
		m_lookat_pt += delta;

		apply();
	}

	void free_camera::move_left(float d)
	{
		vec3f delta;
		cross(delta, m_up, vec3f(m_lookat_pt-m_eye_pos));
		normalize(delta);
		delta *= d;

		m_eye_pos    -= delta;
		m_lookat_pt -= delta;

		apply();
	}

	void free_camera::move_up(float d)
	{
		vec3f delta = m_up;
		normalize(delta);
		delta *= d;

		m_eye_pos    += delta;
		m_lookat_pt += delta;

		apply();
	}

	void free_camera::rotate_right(float angle)
	{		
		vec3f axis = m_up;

		normalize(axis);

		quatf rot;
		setRot(rot, AxisAnglef(angle, axis));

		m_lookat_pt = xform<float>(m_lookat_pt, rot, m_lookat_pt-m_eye_pos) + m_eye_pos;
		apply();
	}

	void free_camera::rotate_up(float angle)
	{
		quatf rot;
		vec3f axis;

		cross(axis, m_up, vec3f(m_lookat_pt-m_eye_pos));
		normalize(axis);
		setRot(rot, AxisAnglef(angle, axis));

		m_lookat_pt = xform<float>(m_lookat_pt, rot, m_lookat_pt-m_eye_pos) + m_eye_pos;
		
		if (!m_fixed_up)
		{
			m_up = xform<float>(m_up, rot, m_up);
		}

		apply();
	}

	void free_camera::rotate_cw(float angle)
	{
		quatf rot;
		vec3f axis = m_lookat_pt-m_eye_pos;

		normalize(axis);
		setRot(rot, AxisAnglef(angle, axis));

		m_up = xform<float>(m_up, rot, m_up);
		apply();
	}

	void free_camera::activate()
	{
		apply();
	}

	void free_camera::apply()
	{
		do_ortho_normal();
		if (m_camera)
		{
			try{
				m_camera->look_at(m_eye_pos,m_lookat_pt,m_up);
				m_camera->activate();
			}
			catch(...){}
		}
	}

	//гарантировать, что вектора "вперед" и "вверх" взаимно пенпедикул€рны и нормированы
	void free_camera::do_ortho_normal()
	{
		normalize(m_up);

		vec3f fwd = m_lookat_pt-m_eye_pos;
		normalize(fwd);

		if (!m_fixed_up)
		{
			//m_up
			vec3f vTmp;
			cross(vTmp, m_up, fwd);
			cross(m_up, fwd, vTmp);
		}

		//m_lookat_pt
		m_lookat_pt = m_eye_pos + fwd;
	}
}
}