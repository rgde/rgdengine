/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: _gmtl_VecOps_h.cpp,v $
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
#include <gmtl-VecOps.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


BOOST_PYTHON_FUNCTION_OVERLOADS(isNormalized_overloads_1_2, gmtl::isNormalized, 1, 2)


}// namespace 


// Module ======================================================================
void _Export_gmtl_VecOps_h()
{
    def("cross", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::cross, return_internal_reference< 1 >());
    def("cross", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::cross, return_internal_reference< 1 >());
    def("cross", (gmtl::Vec<int,3> & (*)(gmtl::Vec<int,3> &, const gmtl::Vec<int,3> &, const gmtl::Vec<int,3> &))&gmtl::cross, return_internal_reference< 1 >());
    def("dot", (double (*)(const gmtl::VecBase<double,4> &, const gmtl::VecBase<double,4> &))&gmtl::dot);
    def("dot", (double (*)(const gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &))&gmtl::dot);
    def("dot", (double (*)(const gmtl::VecBase<double,2> &, const gmtl::VecBase<double,2> &))&gmtl::dot);
    def("dot", (float (*)(const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::dot);
    def("dot", (float (*)(const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::dot);
    def("dot", (float (*)(const gmtl::VecBase<float,2> &, const gmtl::VecBase<float,2> &))&gmtl::dot);
    def("dot", (int (*)(const gmtl::VecBase<int,4> &, const gmtl::VecBase<int,4> &))&gmtl::dot);
    def("dot", (int (*)(const gmtl::VecBase<int,3> &, const gmtl::VecBase<int,3> &))&gmtl::dot);
    def("dot", (int (*)(const gmtl::VecBase<int,2> &, const gmtl::VecBase<int,2> &))&gmtl::dot);
    def("isNormalized", (bool (*)(const gmtl::Vec<double,4> &, const double))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<double,3> &, const double))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<double,2> &, const double))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<float,4> &, const float))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<float,3> &, const float))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<float,2> &, const float))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<int,4> &, const int))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<int,3> &, const int))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<int,2> &, const int))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("length", (double (*)(const gmtl::Vec<double,4> &))&gmtl::length);
    def("length", (double (*)(const gmtl::Vec<double,3> &))&gmtl::length);
    def("length", (double (*)(const gmtl::Vec<double,2> &))&gmtl::length);
    def("length", (float (*)(const gmtl::Vec<float,4> &))&gmtl::length);
    def("length", (float (*)(const gmtl::Vec<float,3> &))&gmtl::length);
    def("length", (float (*)(const gmtl::Vec<float,2> &))&gmtl::length);
    def("length", (int (*)(const gmtl::Vec<int,4> &))&gmtl::length);
    def("length", (int (*)(const gmtl::Vec<int,3> &))&gmtl::length);
    def("length", (int (*)(const gmtl::Vec<int,2> &))&gmtl::length);
    def("lengthSquared", (double (*)(const gmtl::Vec<double,4> &))&gmtl::lengthSquared);
    def("lengthSquared", (double (*)(const gmtl::Vec<double,3> &))&gmtl::lengthSquared);
    def("lengthSquared", (double (*)(const gmtl::Vec<double,2> &))&gmtl::lengthSquared);
    def("lengthSquared", (float (*)(const gmtl::Vec<float,4> &))&gmtl::lengthSquared);
    def("lengthSquared", (float (*)(const gmtl::Vec<float,3> &))&gmtl::lengthSquared);
    def("lengthSquared", (float (*)(const gmtl::Vec<float,2> &))&gmtl::lengthSquared);
    def("lengthSquared", (int (*)(const gmtl::Vec<int,4> &))&gmtl::lengthSquared);
    def("lengthSquared", (int (*)(const gmtl::Vec<int,3> &))&gmtl::lengthSquared);
    def("lengthSquared", (int (*)(const gmtl::Vec<int,2> &))&gmtl::lengthSquared);
    def("lerp", (gmtl::VecBase<double,4> & (*)(gmtl::VecBase<double,4> &, const double &, const gmtl::VecBase<double,4> &, const gmtl::VecBase<double,4> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<double,3> & (*)(gmtl::VecBase<double,3> &, const double &, const gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<double,2> & (*)(gmtl::VecBase<double,2> &, const double &, const gmtl::VecBase<double,2> &, const gmtl::VecBase<double,2> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const float &, const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const float &, const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<float,2> & (*)(gmtl::VecBase<float,2> &, const float &, const gmtl::VecBase<float,2> &, const gmtl::VecBase<float,2> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<int,4> & (*)(gmtl::VecBase<int,4> &, const int &, const gmtl::VecBase<int,4> &, const gmtl::VecBase<int,4> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<int,3> & (*)(gmtl::VecBase<int,3> &, const int &, const gmtl::VecBase<int,3> &, const gmtl::VecBase<int,3> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<int,2> & (*)(gmtl::VecBase<int,2> &, const int &, const gmtl::VecBase<int,2> &, const gmtl::VecBase<int,2> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("normalize", (double (*)(gmtl::Vec<double,4> &))&gmtl::normalize);
    def("normalize", (double (*)(gmtl::Vec<double,3> &))&gmtl::normalize);
    def("normalize", (double (*)(gmtl::Vec<double,2> &))&gmtl::normalize);
    def("normalize", (float (*)(gmtl::Vec<float,4> &))&gmtl::normalize);
    def("normalize", (float (*)(gmtl::Vec<float,3> &))&gmtl::normalize);
    def("normalize", (float (*)(gmtl::Vec<float,2> &))&gmtl::normalize);
    def("normalize", (int (*)(gmtl::Vec<int,4> &))&gmtl::normalize);
    def("normalize", (int (*)(gmtl::Vec<int,3> &))&gmtl::normalize);
    def("normalize", (int (*)(gmtl::Vec<int,2> &))&gmtl::normalize);
    def("reflect", (gmtl::VecBase<double,4> & (*)(gmtl::VecBase<double,4> &, const gmtl::VecBase<double,4> &, const gmtl::Vec<double,4> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<double,3> & (*)(gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &, const gmtl::Vec<double,3> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<double,2> & (*)(gmtl::VecBase<double,2> &, const gmtl::VecBase<double,2> &, const gmtl::Vec<double,2> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &, const gmtl::Vec<float,4> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &, const gmtl::Vec<float,3> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<float,2> & (*)(gmtl::VecBase<float,2> &, const gmtl::VecBase<float,2> &, const gmtl::Vec<float,2> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<int,4> & (*)(gmtl::VecBase<int,4> &, const gmtl::VecBase<int,4> &, const gmtl::Vec<int,4> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<int,3> & (*)(gmtl::VecBase<int,3> &, const gmtl::VecBase<int,3> &, const gmtl::Vec<int,3> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<int,2> & (*)(gmtl::VecBase<int,2> &, const gmtl::VecBase<int,2> &, const gmtl::Vec<int,2> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("isEqual", (bool (*)(const gmtl::VecBase<double,4> &, const gmtl::VecBase<double,4> &, const double))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &, const double))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<double,2> &, const gmtl::VecBase<double,2> &, const double))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &, const float))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &, const float))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<float,2> &, const gmtl::VecBase<float,2> &, const float))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<int,4> &, const gmtl::VecBase<int,4> &, const int))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<int,3> &, const gmtl::VecBase<int,3> &, const int))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<int,2> &, const gmtl::VecBase<int,2> &, const int))&gmtl::isEqual);
}
