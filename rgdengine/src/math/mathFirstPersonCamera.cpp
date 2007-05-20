//mathFirstPersonCamera.cpp
#include "precompiled.h"
#include "math/mathFirstPersonCamera.h"

namespace math
{

    CFirstPersonCamera::CFirstPersonCamera(PCamera pCamera)
    {
        setCamera(pCamera);

        base::lmsg << "CFirstPersonCamera::CFirstPersonCamera()";
        m_vUp       = Vec3f(0.0f, 1.0f, 0.0f);
        m_vEyePt    = Vec3f(0.0f, 0.0f, 0.0f);
        m_vLookatPt = Vec3f(0.0f, 0.0f, 1.0f);
    }

    PFirstPersonCamera CFirstPersonCamera::Create(PCamera pCamera)
    {
        return PFirstPersonCamera(new CFirstPersonCamera(pCamera));
    }

    void CFirstPersonCamera::setPosition(const Vec3f& vUp, const Vec3f& vEyePt, const Vec3f& vLookatPt)
    {
        m_vUp       = vUp;
        m_vEyePt    = vEyePt;
        m_vLookatPt = vLookatPt;
        apply();
    }

    void CFirstPersonCamera::getPosition(Vec3f& vUp, Vec3f& vEyePt, Vec3f& vLookatPt)
    {
        vUp       = m_vUp;
        vEyePt    = m_vEyePt;
        vLookatPt = m_vLookatPt;
    }

    void CFirstPersonCamera::goForward(float delta)
    {
		Vec3f vDir = m_vLookatPt-m_vEyePt;
		normalize(vDir);
		vDir*=delta;
		m_vEyePt+=vDir;
		m_vLookatPt+=vDir;
		apply();
    }

    void CFirstPersonCamera::goLeft(float delta)
    {
		Vec3f vDir = m_vLookatPt-m_vEyePt;
		Vec3f vRight;
		cross(vRight,m_vUp,vDir);
		normalize(vRight);
		vRight*=delta;
		m_vEyePt-=vRight;
		m_vLookatPt-=vRight;
		apply();
    }

    void CFirstPersonCamera::goUp(float delta)
    {
		Vec3f vDir = m_vUp;
		normalize(vDir);
		vDir*=delta;
		m_vEyePt+=vDir;
		m_vLookatPt+=vDir;
		apply();
    }

    void CFirstPersonCamera::rotateRight(float angle)
    {
 		Quatf rot;
        Vec3f vAxis = m_vUp;

		normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        xform<float>(m_vLookatPt, rot, m_vLookatPt-m_vEyePt);
        normalize(m_vLookatPt);
        m_vLookatPt += m_vEyePt;

		apply();
    }

    void CFirstPersonCamera::rotateUp(float angle)
    {
		Quatf rot;
        Vec3f vAxis;
        Vec3f vDir = m_vLookatPt-m_vEyePt;

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

    void CFirstPersonCamera::activate()
    {
        apply();
    }

    void CFirstPersonCamera::apply()
    {
        if (m_pCamera)
        {
            try{
                m_pCamera->lookAt(m_vEyePt,m_vLookatPt,m_vUp);
                m_pCamera->activate();
            }
            catch(...){}
        }
    }

} //namespace math