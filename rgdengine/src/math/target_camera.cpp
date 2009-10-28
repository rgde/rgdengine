#include "precompiled.h"

#include <rgde/math/target_camera.h>

namespace math
{

    target_camera::target_camera(camera_ptr pCamera)
    {
        setCamera(pCamera);

        base::lmsg << "target_camera::target_camera()";
        m_vUp       = Vec3f(0.0f, 1.0f, 0.0f);
        m_vEyePt    = Vec3f(0.0f, 0.0f, 0.0f);
        m_vLookatPt = Vec3f(0.0f, 0.0f, 1.0f);
        doOrthoNormal();
    }

    target_camera_ptr target_camera::create(camera_ptr pCamera)
    {
        return target_camera_ptr(new target_camera(pCamera));
    }

    void target_camera::setPosition(const Vec3f& vUp, const Vec3f& vEyePt, const Vec3f& vLookatPt)
    {
        m_vUp       = vUp;
        m_vEyePt    = vEyePt;
        m_vLookatPt = vLookatPt;
        apply();
    }

    void target_camera::getPosition(Vec3f& vUp, Vec3f& vEyePt, Vec3f& vLookatPt)
    {
        vUp       = m_vUp;
        vEyePt    = m_vEyePt;
        vLookatPt = m_vLookatPt;
    }

    void target_camera::goForward(float delta)
    {
		Vec3f vDir = m_vLookatPt-m_vEyePt;
		Vec3f vDelta = vDir;
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
        Vec3f vAxis = m_vUp;

        normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        m_vEyePt = m_vLookatPt - xform<float>(m_vEyePt, rot, m_vLookatPt-m_vEyePt);
		apply();
    }

    void target_camera::rotateUp(float angle)
    {
		Quatf rot;
        Vec3f vAxis;
        Vec3f vDir = m_vLookatPt-m_vEyePt;

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
        Vec3f vAxis = m_vLookatPt-m_vEyePt;

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
        if (m_pCamera)
        {
            try{
                m_pCamera->lookAt(m_vEyePt,m_vLookatPt,m_vUp);
                m_pCamera->activate();
            }
            catch(...){}
        }
    }

    void target_camera::doOrthoNormal()
    {
        normalize(m_vUp);

        Vec3f vForward = m_vLookatPt-m_vEyePt;
        normalize(vForward);

        //m_vUp
        Vec3f vTmp;
        cross(vTmp, m_vUp, vForward);
        cross(m_vUp, vForward, vTmp);
    }

} //namespace math