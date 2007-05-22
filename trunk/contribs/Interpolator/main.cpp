//main.cpp
#include <rgde/engine.h>
#include "TestInterpolator.h"

#include <rgde/math/mathInterpolators.h>
#include <rgde/math/mathSplines.h>

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    {
        math::FloatLinearInterpolatorf i;
        i.addValue(1,10);
        i.addValue(2,90);
        i.addValue(3,30);
        i.addValue(4,60);

        float t;
        t = i(1.1);
        t = i(1.9);
        t = i(2);
        t = i(2.1);
        t = i(2.9);
        t = i(3);
        t = i(3.1);
        t = i(3.9);
    }

    {
        math::Vec3fCatmulRomSpline s;
        s.m_values.push_back(math::Vec3f(0,0,0));
        s.m_values.push_back(math::Vec3f(1,0,0));
        s.m_values.push_back(math::Vec3f(0,0,0));
        s.m_values.push_back(math::Vec3f(9,0,0));

        math::Vec3f t;
        t = s(0);
        t = s(0.1);
        t = s(0.9);
        t = s(1);
        t = s(1.1);
        t = s(1.9);
        t = s(2);
        t = s(2.1);
        t = s(2.9);
        t = s(3);
    }

    {
        math::Vec3fCatmulRomDistSpline d;
        d.m_values.push_back(math::Vec3f(0,0,0));
        d.m_values.push_back(math::Vec3f(1,0,0));
        d.m_values.push_back(math::Vec3f(0,1,0));
        d.m_values.push_back(math::Vec3f(9,0,5));
        d.apply();

        float t=0;
        float dt;
        while (t < d.length())
        {
            math::Vec3f v1 = d(t);
            t += 0.1f;
            math::Vec3f v2 = d(t);

            math::Vec3fDistance Dist;
            dt = Dist(v1, v2);
        }
    }

    std::auto_ptr<core::IApplication> pApp(core::IApplication::Create(L"Interpolator", 800, 600, false, false));
    pApp->addTask(core::PTask(new core::InputTask(*pApp, 0, false)));
    pApp->addTask(core::PTask(new core::CGameTask(*pApp, 1)));
    pApp->addTask(core::PTask(new core::RenderTask(*pApp, 2)));

    TestInterpolator r;

    pApp->Run();

    return 0;
}