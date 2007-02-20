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
 * File:          $RCSfile: _LineSeg_double.cpp,v $
 * Date modified: $Date: 2006/07/19 14:16:13 $
 * Version:       $Revision: 1.3 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/LineSeg.h>
#include <gmtl/LineSegOps.h>
#include <gmtl/Xforms.h>
#include <gmtl/Output.h>
#include <gmtl-pickle.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_LineSeg_double()
{
    class_< gmtl::LineSeg<double>, bases< gmtl::Ray<double> >  >("LineSegd", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Vec<double,3> & >())
        .def(init< const gmtl::LineSeg<double> & >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Point<double,3> & >())
        .def("getLength", &gmtl::LineSeg<double>::getLength)
        .def("set",
             (gmtl::LineSeg<double>& (gmtl::LineSeg<double>::*)(const gmtl::LineSeg<double>&)) &gmtl::LineSeg<double>::operator=,
             return_internal_reference<1>())
        .def_pickle(gmtlPickle::LineSeg_pickle<double>())
        .def(self == self)
        .def(self != self)
        .def(self *= gmtl::Matrix<double,3,3>())
        .def(self *= gmtl::Matrix<double,4,4>())
        .def(self_ns::str(self))
    ;

}
