//mathFirstPersonCamera.h
#pragma once

#include "math/mathCameraController.h"

namespace math
{
    typedef boost::shared_ptr<class CFirstPersonCamera> PFirstPersonCamera;

    //контроллер камеры аля "камера в FPS"
    class CFirstPersonCamera: public BaseCameraController
    {
        CFirstPersonCamera(PCamera pCamera);

    public:
        static PFirstPersonCamera Create(PCamera pCamera);

        //положение
        void setPosition(const Vec3f& vUp, const Vec3f& vEyePt, const Vec3f& vLookatPt);
        void getPosition(Vec3f& vUp, Vec3f& vEyePt, Vec3f& vLookatPt);

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
