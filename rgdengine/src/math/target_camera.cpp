#include "precompiled.h"

#include <rgde/math/target_camera.h>
#include <rgde/math/camera.h>

namespace math
{

    target_camera::target_camera(camera_ptr camera)
		: base_camera_controller(camera)
    {
        base::lmsg << "target_camera::target_camera()";
        m_up       = vec3f(0.0f, 1.0f, 0.0f);
        m_eye_pos    = vec3f(0.0f, 0.0f, 0.0f);
        m_lookat_pt = vec3f(0.0f, 0.0f, 1.0f);
        do_ortho_normal();
    }

    target_camera_ptr target_camera::create(camera_ptr camera)
    {
        return target_camera_ptr(new target_camera(camera));
    }

    void target_camera::position(const vec3f& up, const vec3f& eye, const vec3f& lookat)
    {
        m_up = up;
        m_eye_pos = eye;
        m_lookat_pt = lookat;
        apply();
    }

    void target_camera::position(vec3f& up, vec3f& eye, vec3f& lookat)
    {
        up = m_up;
        eye = m_eye_pos;
        lookat = m_lookat_pt;
    }

    void target_camera::move_forward(float d)
    {
		vec3f dir = m_lookat_pt-m_eye_pos;
		vec3f delta = dir;
		normalize(delta);
		delta *= d;

		if (length(delta)>length(dir) && d>0)
			return;

        dir -= delta;
		m_eye_pos = m_lookat_pt-dir;
        apply();
    }

    void target_camera::rotate_right(float angle)
    {
		quatf rot;
        vec3f axis = m_up;

        normalize(axis);
        setRot(rot, AxisAnglef(angle, axis));

        m_eye_pos = m_lookat_pt - xform<float>(m_eye_pos, rot, m_lookat_pt-m_eye_pos);
		apply();
    }

    void target_camera::rotate_up(float angle)
    {
		quatf rot;
        vec3f axis;
        vec3f dir = m_lookat_pt-m_eye_pos;

        cross(axis,m_up,dir);
		normalize(axis);
        setRot(rot, AxisAnglef(angle, axis));

        m_eye_pos = m_lookat_pt - xform<float>(m_eye_pos, rot, m_lookat_pt-m_eye_pos);
        xform<float>(m_up, rot, m_up);
		apply();
    }

    void target_camera::rotate_cw(float angle)
    {
		quatf rot;
        vec3f axis = m_lookat_pt-m_eye_pos;

		normalize(axis);
        setRot(rot, AxisAnglef(angle, axis));

        xform<float>(m_up, rot, m_up);
		apply();
    }

    void target_camera::activate()
    {
        apply();
    }

    void target_camera::apply()
    {
        do_ortho_normal();
        if (m_camera)
        {
            try{
                m_camera->lookat(m_eye_pos,m_lookat_pt,m_up);
                m_camera->activate();
            }
            catch(...){}
        }
    }

    void target_camera::do_ortho_normal()
    {
        normalize(m_up);

        vec3f fwd = m_lookat_pt-m_eye_pos;
        normalize(fwd);

        //m_up
        vec3f tmp;
        cross(tmp, m_up, fwd);
        cross(m_up, fwd, tmp);
    }

} //namespace math