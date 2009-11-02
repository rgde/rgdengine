//track_camera.h
#pragma once

#include "camera_controller.h"
#include "track.h"

namespace math
{
    typedef boost::shared_ptr<class path_camera> path_camera_ptr;

    //контроллер камеры аля "камера летающая по некоторой траектории"
    class path_camera: public base_camera_controller
    {
        path_camera(camera_ptr camera);

    public:
        static path_camera_ptr create(camera_ptr camera);

        CTrack m_path;

        //управление
        void  goTo(float position);
        void  goForward(float delta);
        void  goBackward(float delta) {goForward(-delta);}
        float get_pos() {return m_fPosition;}

        void activate();

    private:
        void apply();

        float m_fPosition;
    };
}