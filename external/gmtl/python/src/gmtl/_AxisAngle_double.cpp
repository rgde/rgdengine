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
 * File:          $RCSfile: _AxisAngle_double.cpp,v $
 * Date modified: $Date: 2006/07/19 14:16:13 $
 * Version:       $Revision: 1.3 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/AxisAngle.h>
#include <gmtl/Output.h>
#include <gmtl-pickle.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_AxisAngle_double()
{
    scope* gmtl_AxisAngle_double_scope = new scope(
    class_< gmtl::AxisAngle<double>, bases< gmtl::VecBase<double, 4> > >("AxisAngled", init<  >())
        .def(init< const gmtl::AxisAngle<double> & >())
        .def(init< const double &, const double &, const double &, const double & >())
        .def(init< const double &, const gmtl::Vec<double,3> & >())
        .def("set", (void (gmtl::AxisAngle<double>::*)(const double &, const double &, const double &, const double &) )&gmtl::AxisAngle<double>::set)
        .def("set", (void (gmtl::AxisAngle<double>::*)(const double &, const gmtl::Vec<double,3> &) )&gmtl::AxisAngle<double>::set)
        .def("setAxis", &gmtl::AxisAngle<double>::setAxis)
        .def("setAngle", &gmtl::AxisAngle<double>::setAngle)
        .def("getAxis", &gmtl::AxisAngle<double>::getAxis)
        .def("getAngle", &gmtl::AxisAngle<double>::getAngle, return_value_policy< copy_const_reference >())
        .def("set",
             (gmtl::AxisAngle<double>& (gmtl::AxisAngle<double>::*)(const gmtl::AxisAngle<double>&)) &gmtl::AxisAngle<double>::operator=,
             return_internal_reference<1>())
        .def_pickle(gmtlPickle::AxisAngle_pickle<double>())
        .def(self == self)
        .def(self != self)
        .def(self_ns::str(self))
    );

    enum_< gmtl::AxisAngle<double>::Params >("Params")
        .value("Size", gmtl::AxisAngle<double>::Size)
    ;

    delete gmtl_AxisAngle_double_scope;

}
