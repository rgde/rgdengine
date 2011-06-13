//track_camera_controller.h
#pragma once

#include "camera_controller.h"
#include "track.h"

namespace math
{
    typedef boost::shared_ptr<class track_camera_controller> track_camera_controller_ptr;

    // spline path camera controller
    class track_camera_controller: public base_camera_controller
    {
        track_camera_controller(camera_ptr camera);

    public:
        static track_camera_controller_ptr create(camera_ptr camera);

        track_t m_path;

        void  move_to(float position);
        void  move_forward(float delta);
        void  move_back(float delta) {move_forward(-delta);}
        float position() {return m_position;}

        void activate();

    private:
        void apply();

        float m_position;
    };
}