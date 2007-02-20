/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: _gmtl_CoordOps_h.cpp,v $
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
#include <gmtl-CoordOps.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace  {


BOOST_PYTHON_FUNCTION_OVERLOADS(isEqual_overloads_2_3, gmtl::isEqual, 2, 3)


}// namespace 

// Module ======================================================================
void _Export_gmtl_CoordOps_h()
{
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::EulerAngle<double, gmtl::XYZ> > &, const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::EulerAngle<double, gmtl::XYZ> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::AxisAngle<double> > &, const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::AxisAngle<double> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::AxisAngle<float> > &, const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::AxisAngle<float> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::Quat<double> > &, const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::Quat<double> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::Quat<float> > &, const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::Quat<float> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::EulerAngle<float, gmtl::ZYX> > &, const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::EulerAngle<float, gmtl::ZYX> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::EulerAngle<double, gmtl::ZXY> > &, const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::EulerAngle<double, gmtl::ZXY> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::EulerAngle<double, gmtl::ZYX> > &, const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::EulerAngle<double, gmtl::ZYX> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::Quat<double> > &, const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::Quat<double> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::Quat<float> > &, const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::Quat<float> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::EulerAngle<double, gmtl::ZXY> > &, const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::EulerAngle<double, gmtl::ZXY> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::EulerAngle<double, gmtl::ZYX> > &, const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::EulerAngle<double, gmtl::ZYX> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::EulerAngle<double, gmtl::XYZ> > &, const gmtl::Coord<gmtl::Vec<double, 4>,gmtl::EulerAngle<double, gmtl::XYZ> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::EulerAngle<float, gmtl::ZXY> > &, const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::EulerAngle<float, gmtl::ZXY> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::EulerAngle<float, gmtl::ZYX> > &, const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::EulerAngle<float, gmtl::ZYX> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::AxisAngle<double> > &, const gmtl::Coord<gmtl::Vec<double, 3>,gmtl::AxisAngle<double> > &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::AxisAngle<float> > &, const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::AxisAngle<float> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::EulerAngle<float, gmtl::XYZ> > &, const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::EulerAngle<float, gmtl::XYZ> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::EulerAngle<float, gmtl::ZXY> > &, const gmtl::Coord<gmtl::Vec<float, 3>,gmtl::EulerAngle<float, gmtl::ZXY> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::EulerAngle<float, gmtl::XYZ> > &, const gmtl::Coord<gmtl::Vec<float, 4>,gmtl::EulerAngle<float, gmtl::XYZ> > &, float))&gmtl::isEqual, isEqual_overloads_2_3());
}
