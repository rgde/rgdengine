#include "precompiled.h"

#include <rgde/math/mathTargetCamera.h>

namespace math
{

    CTargetCamera::CTargetCamera(PCamera pCamera)
    {
        setCamera(pCamera);

        base::lmsg << "CTargetCamera::CTargetCamera()";
        m_vUp       = Vec3f(0.0f, 1.0f, 0.0f);
        m_vEyePt    = Vec3f(0.0f, 0.0f, 0.0f);
        m_vLookatPt = Vec3f(0.0f, 0.0f, 1.0f);
        doOrthoNormal();
    }

    PTargetCamera CTargetCamera::Create(PCamera pCamera)
    {
        return PTargetCamera(new CTargetCamera(pCamera));
    }

    void CTargetCamera::setPosition(const Vec3f& vUp, const Vec3f& vEyePt, const Vec3f& vLookatPt)
    {
        m_vUp       = vUp;
        m_vEyePt    = vEyePt;
        m_vLookatPt = vLookatPt;
        apply();
    }

    void CTargetCamera::getPosition(Vec3f& vUp, Vec3f& vEyePt, Vec3f& vLookatPt)
    {
        vUp       = m_vUp;
        vEyePt    = m_vEyePt;
        vLookatPt = m_vLookatPt;
    }

    void CTargetCamera::goForward(float delta)
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

    void CTargetCamera::rotateRight(float angle)
    {
		Quatf rot;
        Vec3f vAxis = m_vUp;

        normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        m_vEyePt = m_vLookatPt - xform<float>(m_vEyePt, rot, m_vLookatPt-m_vEyePt);
		apply();
    }

    void CTargetCamera::rotateUp(float angle)
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

    void CTargetCamera::rotateCW(float angle)
    {
		Quatf rot;
        Vec3f vAxis = m_vLookatPt-m_vEyePt;

		normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        xform<float>(m_vUp, rot, m_vUp);
		apply();
    }

    void CTargetCamera::activate()
    {
        apply();
    }

    void CTargetCamera::apply()
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

    void CTargetCamera::doOrthoNormal()
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