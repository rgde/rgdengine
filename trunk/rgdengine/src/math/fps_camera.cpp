#include "precompiled.h"

#include <rgde/math/fps_camera.h>
#include <rgde/math/camera.h>

namespace math
{

    fps_camera::fps_camera(camera_ptr camera)
		: base_camera_controller(camera)
    {
        base::lmsg << "fps_camera::fps_camera()";
        m_up       = vec3f(0.0f, 1.0f, 0.0f);
        m_eye_pos    = vec3f(0.0f, 0.0f, 0.0f);
        m_lookat_pt = vec3f(0.0f, 0.0f, 1.0f);
    }

    fps_camera_ptr fps_camera::create(camera_ptr camera)
    {
        return fps_camera_ptr(new fps_camera(camera));
    }

    void fps_camera::set_position(const vec3f& up, const vec3f& eye, const vec3f& look_at)
    {
        m_up       = up;
        m_eye_pos    = eye;
        m_lookat_pt = look_at;
        apply();
    }

    void fps_camera::get_pos(vec3f& up, vec3f& eye, vec3f& look_at)
    {
        up       = m_up;
        eye    = m_eye_pos;
        look_at = m_lookat_pt;
    }

    void fps_camera::move_forward(float delta)
    {
		vec3f dir = m_lookat_pt-m_eye_pos;
		normalize(dir);
		dir*=delta;
		m_eye_pos+=dir;
		m_lookat_pt+=dir;
		apply();
    }

    void fps_camera::move_left(float delta)
    {
		vec3f dir = m_lookat_pt-m_eye_pos;
		vec3f vRight;
		cross(vRight,m_up,dir);
		normalize(vRight);
		vRight*=delta;
		m_eye_pos-=vRight;
		m_lookat_pt-=vRight;
		apply();
    }

    void fps_camera::move_up(float delta)
    {
		vec3f dir = m_up;
		normalize(dir);
		dir*=delta;
		m_eye_pos+=dir;
		m_lookat_pt+=dir;
		apply();
    }

    void fps_camera::rotate_right(float angle)
    {
 		quatf rot;
        vec3f axis = m_up;

		normalize(axis);
        setRot(rot, AxisAnglef(angle, axis));

        xform<float>(m_lookat_pt, rot, m_lookat_pt-m_eye_pos);
        normalize(m_lookat_pt);
        m_lookat_pt += m_eye_pos;

		apply();
    }

    void fps_camera::rotate_up(float angle)
    {
		quatf rot;
        vec3f axis;
        vec3f dir = m_lookat_pt-m_eye_pos;

        //боремся с гатством "взгляд вертикально вверх или вниз"
        const float fSmallAngle = 0.01f; //не позволяем приближать направление взгляда к вертикали ближе чем на этот угол
        normalize(dir);
        normalize(m_up);
        float fCurrentAngle = Math::aCos(dot(dir,m_up));
        if (fCurrentAngle + angle <= fSmallAngle)
            angle = -fCurrentAngle + fSmallAngle;
        if (fCurrentAngle + angle >= Math::PI - fSmallAngle)
            angle = Math::PI - fCurrentAngle - fSmallAngle;

        cross(axis,m_up,dir);
		normalize(axis);

        if (length(axis) < 0.1f) return; //иопт! направление взгляда совпало с направлением верха

        setRot(rot, AxisAnglef(angle, axis));

        xform<float>(m_lookat_pt, rot, m_lookat_pt-m_eye_pos);
        normalize(m_lookat_pt);
        m_lookat_pt += m_eye_pos;

		apply();
    }

    void fps_camera::activate()
    {
        apply();
    }

    void fps_camera::apply()
    {
        if (m_camera)
        {
            try{
                m_camera->look_at(m_eye_pos,m_lookat_pt,m_up);
                m_camera->activate();
            }
            catch(...){}
        }
    }

} //namespace math