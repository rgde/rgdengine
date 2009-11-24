//track_camera.h
#pragma once

#include "camera_controller.h"
#include "track.h"

namespace math
{
    typedef boost::shared_ptr<class path_camera> path_camera_ptr;

    // spline path camera controller
    class path_camera: public base_camera_controller
    {
        path_camera(camera_ptr camera);

    public:
        static path_camera_ptr create(camera_ptr camera);

        CTrack m_path;

        void  move_to(float position);
        void  move_forward(float delta);
        void  move_back(float delta) {move_forward(-delta);}
        float get_pos() {return m_position;}

        void activate();

    private:
        void apply();

        float m_position;
    };
}