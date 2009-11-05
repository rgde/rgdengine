#pragma once

#include <rgde/math/camera_controller.h>

namespace math
{

    typedef boost::shared_ptr<class target_camera> target_camera_ptr;

    //контроллер камеры аля "нацеленная камера"
    class target_camera: public base_camera_controller
    {
        target_camera(camera_ptr camera);

    public:
        static target_camera_ptr create(camera_ptr camera);

        //положение
        void set_position(const vec3f& vUp, const vec3f& eye, const vec3f& look_at);
        void get_pos(vec3f& vUp, vec3f& eye, vec3f& look_at);

        //движение
        void goForward(float delta);
        void goBackward(float delta) {goForward(-delta);}

        //вращение
        void rotateRight(float angle);
        void rotateLeft(float angle) {rotateRight(-angle);}
        void rotateUp(float angle);
        void rotateDown(float angle) {rotateUp(-angle);}

	    //наклон
        void rotateCW(float);
        void rotateCCW(float angle) {rotateCW(-angle);}

        void activate();

    private:
        void apply();
        void doOrthoNormal();

        vec3f m_vUp;
        vec3f m_vEyePt;
        vec3f m_vLookatPt;
    };

} //namespace math