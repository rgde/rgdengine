#pragma once

#include <rgde/math/camera_controller.h>

namespace math
{

    typedef boost::shared_ptr<class CTargetCamera> PTargetCamera;

    //контроллер камеры аля "нацеленная камера"
    class CTargetCamera: public BaseCameraController
    {
        CTargetCamera(PCamera pCamera);

    public:
        static PTargetCamera Create(PCamera pCamera);

        //положение
        void setPosition(const Vec3f& vUp, const Vec3f& vEyePt, const Vec3f& vLookatPt);
        void getPosition(Vec3f& vUp, Vec3f& vEyePt, Vec3f& vLookatPt);

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

        Vec3f m_vUp;
        Vec3f m_vEyePt;
        Vec3f m_vLookatPt;
    };

} //namespace math