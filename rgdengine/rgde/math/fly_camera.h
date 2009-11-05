#pragma once

#include <rgde/math/camera_controller.h>

namespace math
{
    typedef boost::shared_ptr<class free_camera> free_camera_ptr;

    //free camera controller
    class free_camera: public base_camera_controller
    {
        free_camera(camera_ptr camera);

    public:
        static free_camera_ptr create(camera_ptr camera);

        //posotion
        void set_position(const vec3f& vUp, const vec3f& eye, const vec3f& look_at);
        void get_pos(vec3f& vUp, vec3f& eye, vec3f& look_at);

        //moving
        void goForward(float delta);
        void goBackward(float delta) {goForward(-delta);}
        void goLeft(float delta);
        void goRight(float delta) {goLeft(-delta);}
        void goUp(float delta);
        void goDown(float delta) {goUp(-delta);}

        //rotation
        void rotateRight(float angle);
        void rotateLeft(float angle) {rotateRight(-angle);}
        void rotate_up(float angle);
        void rotateDown(float angle) {rotate_up(-angle);}
        void rotateCW(float angle);
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