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
        void set_position(const Vec3f& vUp, const Vec3f& vEyePt, const Vec3f& vLookatPt);
        void get_pos(Vec3f& vUp, Vec3f& vEyePt, Vec3f& vLookatPt);

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
        void rotateUp(float angle);
        void rotateDown(float angle) {rotateUp(-angle);}

        void activate();

    private:
        void apply();

        Vec3f m_vUp;
        Vec3f m_vEyePt;
        Vec3f m_vLookatPt;
    };

} //namespace math
