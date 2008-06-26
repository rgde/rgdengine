//interpolators.h
#pragma once

#include "linear_interpolator.h"

namespace math
{

    typedef TLinearInterpolator<double,float>  DoubleLinearInterpolatorf;
    typedef TLinearInterpolator<float,float>   FloatLinearInterpolatorf;
    typedef TLinearInterpolator<int,float>     IntLinearInterpolatorf;
                                              
    typedef TLinearInterpolator<double,double> DoubleLinearInterpolatord;
    typedef TLinearInterpolator<float,double>  FloatLinearInterpolatord;
    typedef TLinearInterpolator<int,double>    IntLinearInterpolatord;

    typedef TLinearInterpolator<Vec2f,float>   Vec2fLinearInterpolatorf;
    typedef TLinearInterpolator<Vec3f,float>   Vec3fLinearInterpolatorf;
    typedef TLinearInterpolator<Vec4f,float>   Vec4fLinearInterpolatorf;

    typedef TLinearInterpolator<Vec2f,double>  Vec2fLinearInterpolatord;
    typedef TLinearInterpolator<Vec3f,double>  Vec3fLinearInterpolatord;
    typedef TLinearInterpolator<Vec4f,double>  Vec4fLinearInterpolatord;

    typedef TLinearInterpolator<Quatf,float>   QuatfLinearInterpolatorf;
    typedef TLinearInterpolator<Quatf,double>  QuatfLinearInterpolatord;

}