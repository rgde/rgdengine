//interpolators.h
#pragma once

#include "linear_interpolator.h"

namespace math
{

    typedef linear_interpolator<double,float>  DoubleLinearInterpolatorf;
    typedef linear_interpolator<float,float>   FloatLinearInterpolatorf;
    typedef linear_interpolator<int,float>     IntLinearInterpolatorf;
                                              
    typedef linear_interpolator<double,double> DoubleLinearInterpolatord;
    typedef linear_interpolator<float,double>  FloatLinearInterpolatord;
    typedef linear_interpolator<int,double>    IntLinearInterpolatord;

    typedef linear_interpolator<vec2f,float>   Vec2fLinearInterpolatorf;
    typedef linear_interpolator<vec3f,float>   Vec3fLinearInterpolatorf;
    typedef linear_interpolator<vec4f,float>   Vec4fLinearInterpolatorf;

    typedef linear_interpolator<vec2f,double>  Vec2fLinearInterpolatord;
    typedef linear_interpolator<vec3f,double>  Vec3fLinearInterpolatord;
    typedef linear_interpolator<vec4f,double>  Vec4fLinearInterpolatord;

    typedef linear_interpolator<quatf,float>   QuatfLinearInterpolatorf;
    typedef linear_interpolator<quatf,double>  QuatfLinearInterpolatord;

}