#pragma once

#include <rgde/math/camera_controller.h>

namespace math
{
    typedef boost::shared_ptr<class fps_camera> fps_camera_ptr;

    //контроллер камеры аля "камера в FPS"
    class fps_camera: public base_camera_controller
    {
        fps_camera(camera_ptr camera);

    public:
        static fps_camera_ptr create(camera_ptr camera);

        //положение
        void set_position(const vec3f& vUp, const vec3f& eye, const vec3f& look_at);
        void get_pos(vec3f& vUp, vec3f& eye, vec3f& look_at);

        //движение
        void goForward(float delta);
        void goBackward(float delta) {goForward(-delta);}
        void goLeft(float delta);
        void goRight(float delta) {goLeft(-delta);}
        void goUp(float delta);
        void goDown(float delta) {goUp(-delta);}

        //вращение
        void rotateRight(float angle);
        void rotateLeft(float angle) {rotateRight(-angle);}
        void rotate_up(float angle);
        void rotateDown(float angle) {rotate_up(-angle);}

        void activate();

    private:
        void apply();

        vec3f m_vUp;
        vec3f m_vEyePt;
        vec3f m_vLookatPt;
    };

} //namespace math
