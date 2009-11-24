//splines.h
#pragma once

//сплайны Кэтмула-Рома с параметризацией по умолчанию
#include "catmulrom.h"
#include "catmulrom_dist.h"

namespace math
{
    //сплайны Кэтмула-Рома с параметризацией по умолчанию
    typedef catm_rom_spline<double>      catm_rom_splined;
    typedef catm_rom_spline<float>       catm_rom_splinef;
    typedef catm_rom_spline<int>         catm_rom_splinei;

    typedef catm_rom_spline<math::vec2f> catm_rom_spline_v2f;
    typedef catm_rom_spline<math::vec3f> catm_rom_spline_v3f;
    typedef catm_rom_spline<math::vec4f> catm_rom_spline_v4f;

    typedef catm_rom_spline<math::quatf> catm_rom_spline_q;

    //функторы для вычисления расстояния между различными видами
    //вершин сплайна (для расспространненных типов вершин)

    //для типа float
    struct distancef
    {
        float operator()(const float &k1, const float &k2)
        {
            return fabs(k2-k1);
        }
    };

    //для типа vec3f
    struct distance_vec
    {
        float operator()(const vec3f &k1, const vec3f &k2)
        {
            return length<float,3>(k2-k1);
        }
    };

    //сплайны Кэтмула-Рома с параметризацией длинной сплайна
    typedef catm_rom_spline_dist<float, distancef> catm_rom_spline_distf;
    typedef catm_rom_spline_dist<vec3f, distance_vec> catm_rom_spline_dist_v3f;

} //namespace math