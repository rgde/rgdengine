#include "precompiled.h"

#include <rgde/math/track_camera.h>

namespace math
{

    CTrackCamera::CTrackCamera(PCamera pCamera): m_fPosition(0)
    {
        setCamera(pCamera);
        base::lmsg << "CTrackCamera::CTrackCamera()";
    }

    PTrackCamera CTrackCamera::Create(PCamera pCamera)
    {
        return PTrackCamera(new CTrackCamera(pCamera));
    }

    void CTrackCamera::goTo(float position)
    {
        m_fPosition = position;
        apply();
    }

    void CTrackCamera::goForward(float delta)
    {
        goTo(m_fPosition+delta);
    }

    void CTrackCamera::activate()
    {
        apply();
    }

    void CTrackCamera::apply()
    {
        if (m_fPosition < 0)
            m_fPosition = 0;
        if (m_fPosition > m_cTrack.getTotalTime())
            m_fPosition = m_cTrack.getTotalTime();

        if (m_pCamera)
        {
            try{
                CTrack::Key key = m_cTrack.getKey(m_fPosition);
                //todo: возможно потребуется ортонормализация векторов [Dub]
                m_pCamera->lookAt(key.m_vEyePt, key.m_vLookatPt, key.m_vUp);
                m_pCamera->activate();
            }
            catch(...){}
        }
    }

}