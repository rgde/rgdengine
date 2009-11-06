//track.h
#pragma once

#include "splines.h"

namespace math
{
    //трек
    class CTrack
    {
    public:
        CTrack() {}

        //ключ трека
        struct Key
        {
		    vec3f m_up;
		    vec3f m_eye_pos;
		    vec3f m_lookat_pt;
            float m_speed;
        };

        //внешний массив с ключами трека
        typedef std::list<Key> List;
        typedef List::iterator iterator;
        typedef List::const_iterator const_iterator;
        List m_values;

        //принять/отменить изменения во внешнем масиве
        void apply();
        void undo();

        //сохранение/загрузка трека
		void load(const std::string& strTrackName);
		void save(const std::string& strTrackName);

        //получение точки трека
        float getTotalTime();
        Key getKey(float t);

    private:
        Vec3fCatmulRomSpline     m_splineUp;
        Vec3fCatmulRomSpline     m_splineEyePt;
        Vec3fCatmulRomSpline     m_splineLookatPt;
        FloatCatmulRomSpline     m_splineSpeed;
        FloatLinearInterpolatorf m_time2parameter;

        CTrack(const CTrack&);
        CTrack& operator=(const CTrack&);
    };
}