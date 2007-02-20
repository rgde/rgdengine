/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: gmtl-LineSegOps.h,v $
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

#ifndef _PYGMTL_LINE_SEG_OPS_H_
#define _PYGMTL_LINE_SEG_OPS_H_

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.

#include <gmtl/LineSeg.h>
#include <gmtl/LineSegOps.h>


#if ! defined(__APPLE__)
namespace gmtl
{
   template float distanceSquared(const gmtl::LineSegf&, const gmtl::Point3f&);
   template double distanceSquared(const gmtl::LineSegd&, const gmtl::Point3d&);

   template gmtl::Point3f findNearestPt(const gmtl::LineSegf&,
                                        const gmtl::Point3f&);
   template gmtl::Point3d findNearestPt(const gmtl::LineSegd&,
                                        const gmtl::Point3d&);

   template float distance(const gmtl::LineSegf&, const gmtl::Point3f&);
   template double distance(const gmtl::LineSegd&, const gmtl::Point3d&);
}
#endif


#endif /* _PYGMTL_LINE_SEG_OPS_H_ */
