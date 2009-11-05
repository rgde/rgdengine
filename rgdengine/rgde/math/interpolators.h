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

    typedef TLinearInterpolator<vec2f,float>   Vec2fLinearInterpolatorf;
    typedef TLinearInterpolator<vec3f,float>   Vec3fLinearInterpolatorf;
    typedef TLinearInterpolator<vec4f,float>   Vec4fLinearInterpolatorf;

    typedef TLinearInterpolator<vec2f,double>  Vec2fLinearInterpolatord;
    typedef TLinearInterpolator<vec3f,double>  Vec3fLinearInterpolatord;
    typedef TLinearInterpolator<vec4f,double>  Vec4fLinearInterpolatord;

    typedef TLinearInterpolator<Quatf,float>   QuatfLinearInterpolatorf;
    typedef TLinearInterpolator<Quatf,double>  QuatfLinearInterpolatord;

}