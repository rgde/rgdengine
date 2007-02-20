/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: gmtl-CoordOps.h,v $
 * Date modified: $Date: 2006/07/19 14:15:55 $
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

#ifndef _PYGMTL_COORD_OPS_H_
#define _PYGMTL_COORD_OPS_H_

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.

#include <gmtl/Coord.h>
#include <gmtl/VecOps.h>
#include <gmtl/EulerAngleOps.h>
#include <gmtl/QuatOps.h>
#include <gmtl/AxisAngleOps.h>
#include <gmtl/CoordOps.h>


#if ! defined(__APPLE__)
namespace gmtl
{
   // With Visual C++, these forward declarations do not work, but since they
   // are only to help out Pyste, we don't really need them.
#ifndef _MSC_VER
   template bool isEqual(const gmtl::Coord3fXYZ&, const gmtl::Coord3fXYZ&,
                         float);
   template bool isEqual(const gmtl::Coord3fZYX&, const gmtl::Coord3fZYX&,
                         float);
   template bool isEqual(const gmtl::Coord3fZXY&, const gmtl::Coord3fZXY&,
                         float);

   template bool isEqual(const gmtl::Coord3dXYZ&, const gmtl::Coord3dXYZ&,
                         double);
   template bool isEqual(const gmtl::Coord3dZYX&, const gmtl::Coord3dZYX&,
                         double);
   template bool isEqual(const gmtl::Coord3dZXY&, const gmtl::Coord3dZXY&,
                         double);

   template bool isEqual(const gmtl::Coord4fXYZ&, const gmtl::Coord4fXYZ&,
                         float);
   template bool isEqual(const gmtl::Coord4fZYX&, const gmtl::Coord4fZYX&,
                         float);
   template bool isEqual(const gmtl::Coord4fZXY&, const gmtl::Coord4fZXY&,
                         float);

   template bool isEqual(const gmtl::Coord4dXYZ&, const gmtl::Coord4dXYZ&,
                         double);
   template bool isEqual(const gmtl::Coord4dZYX&, const gmtl::Coord4dZYX&,
                         double);
   template bool isEqual(const gmtl::Coord4dZXY&, const gmtl::Coord4dZXY&,
                         double);

   template bool isEqual(const gmtl::Coord3fQuat&, const gmtl::Coord3fQuat&,
                         float);
   template bool isEqual(const gmtl::Coord3dQuat&, const gmtl::Coord3dQuat&,
                         double);

   template bool isEqual(const gmtl::Coord4fQuat&, const gmtl::Coord4fQuat&,
                         float);
   template bool isEqual(const gmtl::Coord4dQuat&, const gmtl::Coord4dQuat&,
                         double);

   template bool isEqual(const gmtl::Coord3fAxisAngle&,
                         const gmtl::Coord3fAxisAngle&, float);
   template bool isEqual(const gmtl::Coord3dAxisAngle&,
                         const gmtl::Coord3dAxisAngle&, double);

   template bool isEqual(const gmtl::Coord4fAxisAngle&,
                         const gmtl::Coord4fAxisAngle&, float);
   template bool isEqual(const gmtl::Coord4dAxisAngle&,
                         const gmtl::Coord4dAxisAngle&, double);
#endif
}
#endif


#endif /* _PYGMTL_COORD_OPS_H_ */
