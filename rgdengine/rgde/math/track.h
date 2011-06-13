//track.h
#pragma once

#include "splines.h"

namespace math
{
    //трек
    class track_t
    {
    public:
		track_t(){}
        
        struct key_t
        {
		    vec3f m_up;
		    vec3f m_eye_pos;
		    vec3f m_lookat_pt;
            float m_speed;
        };

        typedef std::list<key_t> keys;
        typedef keys::iterator iterator;
        typedef keys::const_iterator const_iterator;
        keys m_keys;

        //принять/отменить изменения во внешнем масиве
        void apply();
        void undo();

		void load(const std::string& filename);
		void save(const std::string& filename);

        //получение точки трека
        float total_time();
        key_t key(float t);

    private:
        catm_rom_spline_v3f     m_splineUp;
        catm_rom_spline_v3f     m_splineEyePt;
        catm_rom_spline_v3f     m_splineLookatPt;
        catm_rom_splinef     m_splineSpeed;
        FloatLinearInterpolatorf m_time2parameter;

        track_t(const track_t&);
        track_t& operator=(const track_t&);
    };
}