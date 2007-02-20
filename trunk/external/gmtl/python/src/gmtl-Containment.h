/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: gmtl-Containment.h,v $
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

#ifndef _PYGMTL_CONTAINMENT_H_
#define _PYGMTL_CONTAINMENT_H_

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.

#include <gmtl/Containment.h>


#if ! defined(__APPLE__)
namespace gmtl
{
   template bool isInVolume(const gmtl::Spheref&, const gmtl::Point3f&);
   template bool isInVolume(const gmtl::Sphered&, const gmtl::Point3d&);

   template bool isInVolume(const gmtl::Spheref&, const gmtl::Spheref&);
   template bool isInVolume(const gmtl::Sphered&, const gmtl::Sphered&);

   template void extendVolume(gmtl::Spheref&, const gmtl::Point3f&);
   template void extendVolume(gmtl::Sphered&, const gmtl::Point3d&);

   template void extendVolume(gmtl::Spheref&, const gmtl::Spheref&);
   template void extendVolume(gmtl::Sphered&, const gmtl::Sphered&);

   template void makeVolume(gmtl::Spheref&, const std::vector<gmtl::Point3f>&);
   template void makeVolume(gmtl::Sphered&, const std::vector<gmtl::Point3d>&);

   template bool isOnVolume(const gmtl::Spheref&, const gmtl::Point3f&);
   template bool isOnVolume(const gmtl::Sphered&, const gmtl::Point3d&);

   template bool isOnVolume(const gmtl::Spheref&, const gmtl::Point3f&,
                            const float&);
   template bool isOnVolume(const gmtl::Sphered&, const gmtl::Point3d&,
                            const double&);

   template bool isInVolume(const gmtl::AABoxf&, const gmtl::Point3f&);
   template bool isInVolume(const gmtl::AABoxd&, const gmtl::Point3d&);

   template bool isInVolumeExclusive(const gmtl::AABoxf&, const gmtl::Point3f&);
   template bool isInVolumeExclusive(const gmtl::AABoxd&, const gmtl::Point3d&);

   template bool isInVolume(const gmtl::AABoxf&, const gmtl::AABoxf&);
   template bool isInVolume(const gmtl::AABoxd&, const gmtl::AABoxd&);

   template void extendVolume(gmtl::AABoxf&, const gmtl::Point3f&);
   template void extendVolume(gmtl::AABoxd&, const gmtl::Point3d&);

   template void extendVolume(gmtl::AABoxf&, const gmtl::AABoxf&);
   template void extendVolume(gmtl::AABoxd&, const gmtl::AABoxd&);

   template void makeVolume(gmtl::AABoxf&, const gmtl::Spheref&);
   template void makeVolume(gmtl::AABoxd&, const gmtl::Sphered&);
}
#endif


#endif /* _PYGMTL_CONTAINMENT_H_ */
