/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: _gmtl_Containment_h.cpp,v $
 * Date modified: $Date: 2006/07/19 14:16:13 $
 * Version:       $Revision: 1.3 $
 * -----------------------------------------------------------------
 *
 *********************************************************** ggt-head end */
/*************************************************************** ggt-cpr beg
*
* GGT: The Generic Graphics Toolkit
* Copyright (C) 2001,2002 Allen Bierbaum
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
 ************************************************************ ggt-cpr end */

// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl-Containment.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace gmtlWrappers
{

template<typename DATA_TYPE>
void makeVolume(gmtl::Sphere<DATA_TYPE>& container, list pts)
{
   const unsigned int size = extract<unsigned int>(pts.attr("__len__")());
   std::vector< gmtl::Point<DATA_TYPE, 3> > pt_vec(size);

   for ( unsigned int i = 0; i < size; ++i )
   {
#if defined(__GNUC__) && __GNUC__ == 3 && __GNUC_MINOR__ == 2
      gmtl::Point<DATA_TYPE, 3> temp = extract< gmtl::Point<DATA_TYPE, 3> >(pts[i]);
      pt_vec[i] = temp;
#else
      pt_vec[i] = extract< gmtl::Point<DATA_TYPE, 3> >(pts[i]);
#endif
   }

   gmtl::makeVolume(container, pt_vec);
}

}

namespace  {


BOOST_PYTHON_FUNCTION_OVERLOADS(isOnVolume_overloads_2_3, gmtl::isOnVolume, 2, 3)


}// namespace 

// Module ======================================================================
void _Export_gmtl_Containment_h()
{
    def("extendVolume", (void (*)(gmtl::Sphere<double> &, const gmtl::Sphere<double> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<float> &, const gmtl::Sphere<float> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<float> &, const gmtl::Point<float,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<double> &, const gmtl::Point<double,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<double> &, const gmtl::AABox<double> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<float> &, const gmtl::AABox<float> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<double> &, const gmtl::Point<double,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<float> &, const gmtl::Point<float,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("isInVolume", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Sphere<float> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Point<double,3> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Sphere<double> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Point<float,3> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::AABox<double> &, const gmtl::Point<double,3> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::AABox<float> &, const gmtl::Point<float,3> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::AABox<double> &, const gmtl::AABox<double> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::AABox<float> &, const gmtl::AABox<float> &))&gmtl::isInVolume);
    def("isInVolumeExclusive", (bool (*)(const gmtl::AABox<double> &, const gmtl::Point<double,3> &))&gmtl::isInVolumeExclusive);
    def("isInVolumeExclusive", (bool (*)(const gmtl::AABox<float> &, const gmtl::Point<float,3> &))&gmtl::isInVolumeExclusive);
    def("isOnVolume", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Point<double,3> &, const double &))&gmtl::isOnVolume, isOnVolume_overloads_2_3());
    def("isOnVolume", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Point<float,3> &, const float &))&gmtl::isOnVolume, isOnVolume_overloads_2_3());
    def("makeVolume", (void (*)(gmtl::Sphere<float> &, list))&gmtlWrappers::makeVolume, return_internal_reference< 1 >());
    def("makeVolume", (void (*)(gmtl::AABox<float> &, const gmtl::Sphere<float> &))&gmtl::makeVolume, return_internal_reference< 1 >());
    def("makeVolume", (void (*)(gmtl::Sphere<double> &, list))&gmtlWrappers::makeVolume, return_internal_reference< 1 >());
    def("makeVolume", (void (*)(gmtl::AABox<double> &, const gmtl::Sphere<double> &))&gmtl::makeVolume, return_internal_reference< 1 >());
}
