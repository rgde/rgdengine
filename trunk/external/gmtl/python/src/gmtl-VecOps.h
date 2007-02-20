/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: gmtl-VecOps.h,v $
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

#ifndef _PYGMTL_VEC_OPS_H_
#define _PYGMTL_VEC_OPS_H_

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.

#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>


#if ! defined(__APPLE__)
namespace gmtl
{
   typedef gmtl::VecBase<float, 3> VecBase3f;
   typedef gmtl::VecBase<double, 3> VecBase3d;
   typedef gmtl::VecBase<float, 4> VecBase4f;
   typedef gmtl::VecBase<double, 4> VecBase4d;

   template float dot(const gmtl::VecBase<float, 3>&,
                      const gmtl::VecBase<float, 3>&);
   template float dot(const gmtl::VecBase<float, 4>&,
                      const gmtl::VecBase<float, 4>&);

   template float length(const gmtl::Vec3f&);
   template float length(const gmtl::Vec4f&);

   template float lengthSquared(const gmtl::Vec3f&);
   template float lengthSquared(const gmtl::Vec4f&);

   template float normalize(gmtl::Vec3f&);
   template float normalize(gmtl::Vec4f&);

   template bool isNormalized(const gmtl::Vec3f&, const float);
   template bool isNormalized(const gmtl::Vec4f&, const float);

   template gmtl::Vec3i& cross(gmtl::Vec3i&, const gmtl::Vec3i&,
                               const gmtl::Vec3i&);
   template gmtl::Vec3d& cross(gmtl::Vec3d&, const gmtl::Vec3d&,
                               const gmtl::Vec3d&);
   template gmtl::Vec3f& cross(gmtl::Vec3f&, const gmtl::Vec3f&,
                               const gmtl::Vec3f&);

   template VecBase3f& reflect(VecBase3f&, const VecBase3f&, const Vec3f&);
   template VecBase3d& reflect(VecBase3d&, const VecBase3d&, const Vec3d&);
   template VecBase4f& reflect(VecBase4f&, const VecBase4f&, const Vec4f&);
   template VecBase4d& reflect(VecBase4d&, const VecBase4d&, const Vec4d&);

   template VecBase<float, 2>& lerp(VecBase<float, 2>&, const float&,
                                    const VecBase<float, 2>&,
                                    const VecBase<float, 2>&);
   template VecBase<double, 2>& lerp(VecBase<double, 2>&, const double&,
                                     const VecBase<double, 2>&,
                                     const VecBase<double, 2>&);
   template VecBase3f& lerp(VecBase3f&, const float&, const VecBase3f&,
                            const VecBase3f&);
   template VecBase3d& lerp(VecBase3d&, const double&, const VecBase3d&,
                            const VecBase3d&);
   template VecBase4f& lerp(VecBase4f&, const float&, const VecBase4f&,
                            const VecBase4f&);
   template VecBase4d& lerp(VecBase4d&, const double&, const VecBase4d&,
                            const VecBase4d&);

   typedef gmtl::VecBase<float, 3> VecBase3f;
   typedef gmtl::VecBase<double, 3> VecBase3d;
   typedef gmtl::VecBase<float, 4> VecBase4f;
   typedef gmtl::VecBase<double, 4> VecBase4d;

   template bool isEqual(const gmtl::VecBase<float, 2>&,
                         const gmtl::VecBase<float, 2>&, const float);
   template bool isEqual(const gmtl::VecBase<double, 2>&,
                         const gmtl::VecBase<double, 2>&, const double);
   template bool isEqual(const gmtl::VecBase3f&, const gmtl::VecBase3f&,
                         const float);
   template bool isEqual(const gmtl::VecBase3d&, const gmtl::VecBase3d&,
                         const double);
   template bool isEqual(const gmtl::VecBase4f&, const gmtl::VecBase4f&,
                         const float);
   template bool isEqual(const gmtl::VecBase4d&, const gmtl::VecBase4d&,
                         const double);
}
#endif


#endif /* _PYGMTL_VEC_OPS_H_ */
