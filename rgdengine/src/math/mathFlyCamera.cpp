//mathFlyCamera.cpp
#include "precompiled.h"
#include "math/mathFlyCamera.h"

namespace math
{

    CFlyCamera::CFlyCamera(PCamera pCamera)
    {
        setCamera(pCamera);

        base::lmsg << "CFlyCamera::CFlyCamera()";
        m_vUp       = Vec3f(0.0f, 1.0f, 0.0f);
        m_vEyePt    = Vec3f(0.0f, 0.0f, 0.0f);
        m_vLookatPt = Vec3f(0.0f, 0.0f, 1.0f);
        doOrthoNormal();
    }

    PFlyCamera CFlyCamera::Create(PCamera pCamera)
    {
        return PFlyCamera(new CFlyCamera(pCamera));
    }

    void CFlyCamera::setPosition(const Vec3f& vUp, const Vec3f& vEyePt, const Vec3f& vLookatPt)
    {
        m_vUp       = vUp;
        m_vEyePt    = vEyePt;
        m_vLookatPt = vLookatPt;
        apply();
    }

    void CFlyCamera::getPosition(Vec3f& vUp, Vec3f& vEyePt, Vec3f& vLookatPt)
    {
        vUp       = m_vUp;
        vEyePt    = m_vEyePt;
        vLookatPt = m_vLookatPt;
    }

    void CFlyCamera::goForward(float delta)
    {
        Vec3f vDelta = m_vLookatPt-m_vEyePt;
        normalize(vDelta);
        vDelta *= delta;

        m_vEyePt    += vDelta;
        m_vLookatPt += vDelta;

        apply();
    }

    void CFlyCamera::goLeft(float delta)
    {
        Vec3f vDelta;
        cross(vDelta, m_vUp, Vec3f(m_vLookatPt-m_vEyePt));
        normalize(vDelta);
        vDelta *= delta;

        m_vEyePt    -= vDelta;
        m_vLookatPt -= vDelta;

        apply();
    }

    void CFlyCamera::goUp(float delta)
    {
        Vec3f vDelta = m_vUp;
        normalize(vDelta);
        vDelta *= delta;

        m_vEyePt    += vDelta;
        m_vLookatPt += vDelta;

        apply();
    }

    void CFlyCamera::rotateRight(float angle)
    {
        Quatf rot;
        Vec3f vAxis = m_vUp;

        normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        m_vLookatPt = xform<float>(m_vLookatPt, rot, m_vLookatPt-m_vEyePt) + m_vEyePt;
        apply();
    }

    void CFlyCamera::rotateUp(float angle)
    {
        Quatf rot;
        Vec3f vAxis;

        cross(vAxis, m_vUp, Vec3f(m_vLookatPt-m_vEyePt));
        normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        m_vLookatPt = xform<float>(m_vLookatPt, rot, m_vLookatPt-m_vEyePt) + m_vEyePt;
        m_vUp = xform<float>(m_vUp, rot, m_vUp);
        apply();
    }

    void CFlyCamera::rotateCW(float angle)
    {
        Quatf rot;
        Vec3f vAxis = m_vLookatPt-m_vEyePt;

        normalize(vAxis);
        setRot(rot, AxisAnglef(angle, vAxis));

        m_vUp = xform<float>(m_vUp, rot, m_vUp);
        apply();
    }

    void CFlyCamera::activate()
    {
        apply();
    }

    void CFlyCamera::apply()
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

    //гарантировать, что вектора "вперед" и "вверх" взаимно пенпедикул€рны и нормированы
    void CFlyCamera::doOrthoNormal()
    {
        normalize(m_vUp);

        Vec3f vForward = m_vLookatPt-m_vEyePt;
        normalize(vForward);

        //m_vUp
        Vec3f vTmp;
        cross(vTmp, m_vUp, vForward);
        cross(m_vUp, vForward, vTmp);

        //m_vLookatPt
        m_vLookatPt = m_vEyePt + vForward;
    }

} //namespace math