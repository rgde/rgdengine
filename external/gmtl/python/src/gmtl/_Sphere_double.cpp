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
 * File:          $RCSfile: _Sphere_double.cpp,v $
 * Date modified: $Date: 2006/07/19 14:16:13 $
 * Version:       $Revision: 1.3 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Sphere.h>
#include <gmtl/SphereOps.h>
#include <gmtl/Output.h>
#include <gmtl-pickle.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Sphere_double()
{
    class_< gmtl::Sphere<double> >("Sphered", init<  >())
        .def(init< const gmtl::Point<double,3> &, const double & >())
        .def(init< const gmtl::Sphere<double> & >())
        .def_readwrite("center", &gmtl::Sphere<double>::mCenter)
        .def_readwrite("radius", &gmtl::Sphere<double>::mRadius)
        .def("getCenter", &gmtl::Sphere<double>::getCenter, return_value_policy< copy_const_reference >())
        .def("getRadius", &gmtl::Sphere<double>::getRadius, return_value_policy< copy_const_reference >())
        .def("setCenter", &gmtl::Sphere<double>::setCenter)
        .def("setRadius", &gmtl::Sphere<double>::setRadius)
        .def("set",
             (gmtl::Sphere<double>& (gmtl::Sphere<double>::*)(const gmtl::Sphere<double>&)) &gmtl::Sphere<double>::operator=,
             return_internal_reference<1>())
        .def_pickle(gmtlPickle::Sphere_pickle<double>())
        .def(self == self)
        .def(self != self)
        .def(self_ns::str(self))
    ;

}
