/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: _Ray_double.cpp,v $
 * Date modified: $Date: 2006/07/19 14:16:13 $
 * Version:       $Revision: 1.3 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Ray.h>
#include <gmtl/RayOps.h>
#include <gmtl/Xforms.h>
#include <gmtl/Output.h>
#include <gmtl-pickle.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Ray_double()
{
    class_< gmtl::Ray<double> >("Rayd", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Vec<double,3> & >())
        .def(init< const gmtl::Ray<double> & >())
        .def_readwrite("origin", &gmtl::Ray<double>::mOrigin)
        .def_readwrite("dir", &gmtl::Ray<double>::mDir)
        .def("getOrigin", &gmtl::Ray<double>::getOrigin, return_value_policy< copy_const_reference >())
        .def("setOrigin", &gmtl::Ray<double>::setOrigin)
        .def("getDir", &gmtl::Ray<double>::getDir, return_value_policy< copy_const_reference >())
        .def("setDir", &gmtl::Ray<double>::setDir)
        .def("set",
             (gmtl::Ray<double>& (gmtl::Ray<double>::*)(const gmtl::Ray<double>&)) &gmtl::Ray<double>::operator=,
             return_internal_reference<1>())
        .def_pickle(gmtlPickle::Ray_pickle<double>())
        .def(self == self)
        .def(self != self)
        .def(self *= gmtl::Matrix<double,3,3>())
        .def(self *= gmtl::Matrix<double,4,4>())
        .def(self_ns::str(self))
    ;

}
