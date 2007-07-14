/*

@author PC
*/
#include "InterpolationMethods.h"

#define DECLARE_INTERPOLATOR_TYPE(name, TValue, TPosition, TMethod) typedef TInterpolator<TValue, TPosition, TMethod<TValue, TPosition> > name;


DECLARE_INTERPOLATOR_TYPE(DoubleLinearInterpolatorf, double, float, LinearInterpolationMethod)
DECLARE_INTERPOLATOR_TYPE(FloatLinearInterpolatorf, float, float, LinearInterpolationMethod)
DECLARE_INTERPOLATOR_TYPE(IntLinearInterpolatorf, int, float, LinearInterpolationMethod)

DECLARE_INTERPOLATOR_TYPE(DoubleLinearInterpolatord, double, double, LinearInterpolationMethod)
DECLARE_INTERPOLATOR_TYPE(FloatLinearInterpolatord, float, double, LinearInterpolationMethod)
DECLARE_INTERPOLATOR_TYPE(IntLinearInterpolatord, int, double, LinearInterpolationMethod)

DECLARE_INTERPOLATOR_TYPE(Vec2fLinearInterpolatorf, math::Vec2f, float, LinearInterpolationMethod)
DECLARE_INTERPOLATOR_TYPE(Vec3fLinearInterpolatorf, math::Vec3f, float, LinearInterpolationMethod)
DECLARE_INTERPOLATOR_TYPE(Vec4fLinearInterpolatorf, math::Vec4f, float, LinearInterpolationMethod)

DECLARE_INTERPOLATOR_TYPE(Vec2fLinearInterpolatord, math::Vec2f, double, LinearInterpolationMethod)
DECLARE_INTERPOLATOR_TYPE(Vec3fLinearInterpolatord, math::Vec3f, double, LinearInterpolationMethod)
DECLARE_INTERPOLATOR_TYPE(Vec4fLinearInterpolatord, math::Vec4f, double, LinearInterpolationMethod)

DECLARE_INTERPOLATOR_TYPE(QuatfLinearInterpolatorf, math::Quatf, float, LinearInterpolationMethod)

DECLARE_INTERPOLATOR_TYPE(QuatfLinearInterpolatord, math::Quatf, double, LinearInterpolationMethod)