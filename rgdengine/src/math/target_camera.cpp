#include "precompiled.h"

#include <rgde/math/target_camera.h>

namespace math
{

    target_camera::target_camera(camera_ptr camera)
    {
        set_camera(camera);

        base::lmsg << "target_camera::target_camera()";
        m_vUp       = vec3f(0.0f, 1.0f, 0.0f);
        m_vEyePt    = vec3f(0.0f, 0.0f, 0.0f);
        m_vLookatPt = vec3f(0.0f, 0.0f, 1.0f);
        doOrthoNormal();
    }

    target_camera_ptr target_camera::create(camera_ptr camera)
    {
        return target_camera_ptr(new target_camera(camera));
    }

    void target_camera::set_position(const vec3f& vUp, const vec3f& eye, const vec3f& look_at)
    {
        m_vUp       = vUp;
        m_vEyePt    = eye;
        m_vLookatPt = look_at;
        apply();
    }

    void target_camera::get_pos(vec3f& vUp, vec3f& eye, vec3f& look_at)
    {
        vUp       = m_vUp;
        eye    = m_vEyePt;
        look_at = m_vLookatPt;
    }

    void target_camera::goForward(float delta)
    {
		vec3f vDir = m_vLookatPt-m_vEyePt;
		vec3f vDelta = vDir;
		normalize(vDelta);
		vDelta *= delta;

		if (length(vDelta)>length(vDir) && delta>0)
			return;

        vDir -= vDelta;
		m_vEyePt = m_vLookatPt-vDir;
        apply();
    }

    void target_camera::rotateRight(float angle)
    {
		Quatf rot;
        vec3f vAxis = m_vUp;

        normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        m_vEyePt = m_vLookatPt - xform<float>(m_vEyePt, rot, m_vLookatPt-m_vEyePt);
		apply();
    }

    void target_camera::rotate_up(float angle)
    {
		Quatf rot;
        vec3f vAxis;
        vec3f vDir = m_vLookatPt-m_vEyePt;

        cross(vAxis,m_vUp,vDir);
		normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        m_vEyePt = m_vLookatPt - xform<float>(m_vEyePt, rot, m_vLookatPt-m_vEyePt);
        xform<float>(m_vUp, rot, m_vUp);
		apply();
    }

    void target_camera::rotateCW(float angle)
    {
		Quatf rot;
        vec3f vAxis = m_vLookatPt-m_vEyePt;

		normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        xform<float>(m_vUp, rot, m_vUp);
		apply();
    }

    void target_camera::activate()
    {
        apply();
    }

    void target_camera::apply()
    {
        doOrthoNormal();
        if (m_camera)
        {
            try{
                m_camera->look_at(m_vEyePt,m_vLookatPt,m_vUp);
                m_camera->activate();
            }
            catch(...){}
        }
    }

    void target_camera::doOrthoNormal()
    {
        normalize(m_vUp);

        vec3f vForward = m_vLookatPt-m_vEyePt;
        normalize(vForward);

        //m_vUp
        vec3f vTmp;
        cross(vTmp, m_vUp, vForward);
        cross(m_vUp, vForward, vTmp);
    }

} //namespace math