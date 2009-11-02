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
        void set_position(const Vec3f& vUp, const Vec3f& vEyePt, const Vec3f& vLookatPt);
        void getPosition(Vec3f& vUp, Vec3f& vEyePt, Vec3f& vLookatPt);

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
        void rotateUp(float angle);
        void rotateDown(float angle) {rotateUp(-angle);}
        void rotateCW(float angle);
        void rotateCCW(float angle) {rotateCW(-angle);}

        void activate();

    private:
        void apply();
        void doOrthoNormal();

        Vec3f m_vUp;
        Vec3f m_vEyePt;
        Vec3f m_vLookatPt;
    };

} //namespace math