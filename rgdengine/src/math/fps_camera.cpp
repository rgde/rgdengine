#include "precompiled.h"

#include <rgde/math/fps_camera.h>

namespace math
{

    fps_camera::fps_camera(camera_ptr camera)
    {
        set_camera(camera);

        base::lmsg << "fps_camera::fps_camera()";
        m_vUp       = vec3f(0.0f, 1.0f, 0.0f);
        m_vEyePt    = vec3f(0.0f, 0.0f, 0.0f);
        m_vLookatPt = vec3f(0.0f, 0.0f, 1.0f);
    }

    fps_camera_ptr fps_camera::create(camera_ptr camera)
    {
        return fps_camera_ptr(new fps_camera(camera));
    }

    void fps_camera::set_position(const vec3f& vUp, const vec3f& eye, const vec3f& look_at)
    {
        m_vUp       = vUp;
        m_vEyePt    = eye;
        m_vLookatPt = look_at;
        apply();
    }

    void fps_camera::get_pos(vec3f& vUp, vec3f& eye, vec3f& look_at)
    {
        vUp       = m_vUp;
        eye    = m_vEyePt;
        look_at = m_vLookatPt;
    }

    void fps_camera::goForward(float delta)
    {
		vec3f vDir = m_vLookatPt-m_vEyePt;
		normalize(vDir);
		vDir*=delta;
		m_vEyePt+=vDir;
		m_vLookatPt+=vDir;
		apply();
    }

    void fps_camera::goLeft(float delta)
    {
		vec3f vDir = m_vLookatPt-m_vEyePt;
		vec3f vRight;
		cross(vRight,m_vUp,vDir);
		normalize(vRight);
		vRight*=delta;
		m_vEyePt-=vRight;
		m_vLookatPt-=vRight;
		apply();
    }

    void fps_camera::goUp(float delta)
    {
		vec3f vDir = m_vUp;
		normalize(vDir);
		vDir*=delta;
		m_vEyePt+=vDir;
		m_vLookatPt+=vDir;
		apply();
    }

    void fps_camera::rotateRight(float angle)
    {
 		Quatf rot;
        vec3f vAxis = m_vUp;

		normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        xform<float>(m_vLookatPt, rot, m_vLookatPt-m_vEyePt);
        normalize(m_vLookatPt);
        m_vLookatPt += m_vEyePt;

		apply();
    }

    void fps_camera::rotateUp(float angle)
    {
		Quatf rot;
        vec3f vAxis;
        vec3f vDir = m_vLookatPt-m_vEyePt;

        //боремся с гатством "взгляд вертикально вверх или вниз"
        const float fSmallAngle = 0.01f; //не позволяем приближать направление взгляда к вертикали ближе чем на этот угол
        normalize(vDir);
        normalize(m_vUp);
        float fCurrentAngle = Math::aCos(dot(vDir,m_vUp));
        if (fCurrentAngle + angle <= fSmallAngle)
            angle = -fCurrentAngle + fSmallAngle;
        if (fCurrentAngle + angle >= Math::PI - fSmallAngle)
            angle = Math::PI - fCurrentAngle - fSmallAngle;

        cross(vAxis,m_vUp,vDir);
		normalize(vAxis);

        if (length(vAxis) < 0.1f) return; //иопт! направление взгляда совпало с направлением верха

        setRot(rot, AxisAnglef(angle, vAxis));

        xform<float>(m_vLookatPt, rot, m_vLookatPt-m_vEyePt);
        normalize(m_vLookatPt);
        m_vLookatPt += m_vEyePt;

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
                m_camera->look_at(m_vEyePt,m_vLookatPt,m_vUp);
                m_camera->activate();
            }
            catch(...){}
        }
    }

} //namespace math