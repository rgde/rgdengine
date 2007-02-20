/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: gmtl-QuatOps.h,v $
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

#ifndef _PYGMTL_QUAT_OPS_H_
#define _PYGMTL_QUAT_OPS_H_

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.

#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>


#if ! defined(__APPLE__)
namespace gmtl
{
   template gmtl::Quatf& mult(gmtl::Quatf&, const gmtl::Quatf&,
                              const gmtl::Quatf&);
   template gmtl::Quatd& mult(gmtl::Quatd&, const gmtl::Quatd&,
                              const gmtl::Quatd&);

   template gmtl::Quatf& mult(gmtl::Quatf&, const gmtl::Quatf&, float);
   template gmtl::Quatd& mult(gmtl::Quatd&, const gmtl::Quatd&, double);

   template gmtl::Quatf& negate(gmtl::Quatf&);
   template gmtl::Quatd& negate(gmtl::Quatd&);

   template gmtl::Quatf& div(gmtl::Quatf&, const gmtl::Quatf&, gmtl::Quatf);
   template gmtl::Quatd& div(gmtl::Quatd&, const gmtl::Quatd&, gmtl::Quatd);

   template gmtl::Quatf& div(gmtl::Quatf&, const gmtl::Quatf&, float);
   template gmtl::Quatd& div(gmtl::Quatd&, const gmtl::Quatd&, double);

   template gmtl::Quatf& add(gmtl::Quatf&, const gmtl::Quatf&,
                             const gmtl::Quatf&);
   template gmtl::Quatd& add(gmtl::Quatd&, const gmtl::Quatd&,
                             const gmtl::Quatd&);

   template gmtl::Quatf& sub(gmtl::Quatf&, const gmtl::Quatf&,
                             const gmtl::Quatf&);
   template gmtl::Quatd& sub(gmtl::Quatd&, const gmtl::Quatd&,
                             const gmtl::Quatd&);

   template float dot(const gmtl::Quatf&, const gmtl::Quatf&);
   template double dot(const gmtl::Quatd&, const gmtl::Quatd&);

   template float lengthSquared(const gmtl::Quatf&);
   template double lengthSquared(const gmtl::Quatd&);

   template float length(const gmtl::Quatf&);
   template double length(const gmtl::Quatd&);

   template gmtl::Quatf& normalize(gmtl::Quatf&);
   template gmtl::Quatd& normalize(gmtl::Quatd&);

   template bool isNormalized(const gmtl::Quatf&, const float);
   template bool isNormalized(const gmtl::Quatd&, const double);

   template gmtl::Quatf& conj(gmtl::Quatf&);
   template gmtl::Quatd& conj(gmtl::Quatd&);

   template gmtl::Quatf& invert(gmtl::Quatf&);
   template gmtl::Quatd& invert(gmtl::Quatd&);

   template gmtl::Quatf& exp(gmtl::Quatf&);
   template gmtl::Quatd& exp(gmtl::Quatd&);

   template gmtl::Quatf& log(gmtl::Quatf&);
   template gmtl::Quatd& log(gmtl::Quatd&);

   template void squad(gmtl::Quatf&, float, const gmtl::Quatf&,
                       const gmtl::Quatf&, const gmtl::Quatf&,
                       const gmtl::Quatf&);
   template void squad(gmtl::Quatd&, double, const gmtl::Quatd&,
                       const gmtl::Quatd&, const gmtl::Quatd&,
                       const gmtl::Quatd&);

   template gmtl::Quatf& slerp(gmtl::Quatf&, const float, const gmtl::Quatf&,
                               const gmtl::Quatf&, bool);
   template gmtl::Quatd& slerp(gmtl::Quatd&, const double, const gmtl::Quatd&,
                               const gmtl::Quatd&, bool);

   template Quatf& lerp(Quatf&, const float, const Quatf&, const Quatf&);
   template Quatd& lerp(Quatd&, const double, const Quatd&, const Quatd&);

   template bool isEquiv(const gmtl::Quatf&, const gmtl::Quatf&, float);
   template bool isEquiv(const gmtl::Quatd&, const gmtl::Quatd&, double);

   template bool isEqual(const gmtl::Quatf&, const gmtl::Quatf&, float);
   template bool isEqual(const gmtl::Quatd&, const gmtl::Quatd&, double);
}
#endif


#endif /* _PYGMTL_QUAT_OPS_H_ */
