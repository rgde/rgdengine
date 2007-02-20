/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: _gmtl_Xforms_h.cpp,v $
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
#include <gmtl-Xforms.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_gmtl_Xforms_h()
{
    def("xform", (gmtl::LineSeg<float> & (*)(gmtl::LineSeg<float> &, const gmtl::Matrix<float,4,4> &, const gmtl::LineSeg<float> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::LineSeg<double> & (*)(gmtl::LineSeg<double> &, const gmtl::Matrix<double,4,4> &, const gmtl::LineSeg<double> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Ray<float> & (*)(gmtl::Ray<float> &, const gmtl::Matrix<float,4,4> &, const gmtl::Ray<float> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Ray<double> & (*)(gmtl::Ray<double> &, const gmtl::Matrix<double,4,4> &, const gmtl::Ray<double> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,4> & (*)(gmtl::Point<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Point<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,3> & (*)(gmtl::Point<float,3> &, const gmtl::Matrix<float,4,4> &, const gmtl::Point<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,4> & (*)(gmtl::Point<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Point<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,4> & (*)(gmtl::Point<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Point<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,4> & (*)(gmtl::Vec<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,4> & (*)(gmtl::Vec<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,4> & (*)(gmtl::Vec<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,4> & (*)(gmtl::Vec<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,3,3> &, const gmtl::Vec<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Vec<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,2> & (*)(gmtl::Vec<double,2> &, const gmtl::Matrix<double,3,3> &, const gmtl::Vec<double,2> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,2> & (*)(gmtl::Vec<float,2> &, const gmtl::Matrix<float,3,3> &, const gmtl::Vec<float,2> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::VecBase<double,3> & (*)(gmtl::VecBase<double,3> &, const gmtl::Quat<double> &, const gmtl::VecBase<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,4> & (*)(gmtl::Point<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Point<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,3> & (*)(gmtl::Point<double,3> &, const gmtl::Matrix<double,4,4> &, const gmtl::Point<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,3> & (*)(gmtl::Point<double,3> &, const gmtl::Matrix<double,3,3> &, const gmtl::Point<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,3> & (*)(gmtl::Point<float,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Point<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const gmtl::Quat<float> &, const gmtl::VecBase<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,2> & (*)(gmtl::Point<double,2> &, const gmtl::Matrix<double,3,3> &, const gmtl::Point<double,2> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,2> & (*)(gmtl::Point<float,2> &, const gmtl::Matrix<float,3,3> &, const gmtl::Point<float,2> &))&gmtl::xform, return_internal_reference< 1 >());
}
