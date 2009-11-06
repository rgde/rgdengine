#include "precompiled.h"

#include <rgde/math/target_camera.h>

namespace math
{

    target_camera::target_camera(camera_ptr camera)
    {
        set_camera(camera);

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

    void target_camera::set_position(const vec3f& up, const vec3f& eye, const vec3f& look_at)
    {
        m_up       = up;
        m_eye_pos    = eye;
        m_lookat_pt = look_at;
        apply();
    }

    void target_camera::get_pos(vec3f& up, vec3f& eye, vec3f& look_at)
    {
        up       = m_up;
        eye    = m_eye_pos;
        look_at = m_lookat_pt;
    }

    void target_camera::move_forward(float delta)
    {
		vec3f vDir = m_lookat_pt-m_eye_pos;
		vec3f vDelta = vDir;
		normalize(vDelta);
		vDelta *= delta;

		if (length(vDelta)>length(vDir) && delta>0)
			return;

        vDir -= vDelta;
		m_eye_pos = m_lookat_pt-vDir;
        apply();
    }

    void target_camera::rotate_right(float angle)
    {
		quatf rot;
        vec3f vAxis = m_up;

        normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        m_eye_pos = m_lookat_pt - xform<float>(m_eye_pos, rot, m_lookat_pt-m_eye_pos);
		apply();
    }

    void target_camera::rotate_up(float angle)
    {
		quatf rot;
        vec3f vAxis;
        vec3f vDir = m_lookat_pt-m_eye_pos;

        cross(vAxis,m_up,vDir);
		normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        m_eye_pos = m_lookat_pt - xform<float>(m_eye_pos, rot, m_lookat_pt-m_eye_pos);
        xform<float>(m_up, rot, m_up);
		apply();
    }

    void target_camera::rotate_cw(float angle)
    {
		quatf rot;
        vec3f vAxis = m_lookat_pt-m_eye_pos;

		normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

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
                m_camera->look_at(m_eye_pos,m_lookat_pt,m_up);
                m_camera->activate();
            }
            catch(...){}
        }
    }

    void target_camera::do_ortho_normal()
    {
        normalize(m_up);

        vec3f vForward = m_lookat_pt-m_eye_pos;
        normalize(vForward);

        //m_up
        vec3f vTmp;
        cross(vTmp, m_up, vForward);
        cross(m_up, vForward, vTmp);
    }

} //namespace math