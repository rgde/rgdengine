/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: gmtl-MatrixOps.h,v $
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

#ifndef _PYGMTL_MATRIX_OPS_H_
#define _PYGMTL_MATRIX_OPS_H_

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>


#if ! defined(__APPLE__)
namespace gmtl
{
   template gmtl::Matrix44f& identity(gmtl::Matrix44f&);
   template gmtl::Matrix33f& identity(gmtl::Matrix33f&);

   template gmtl::Matrix44f& zero(gmtl::Matrix44f&);
   template gmtl::Matrix33f& zero(gmtl::Matrix33f&);

   template gmtl::Matrix44f& mult(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                  const gmtl::Matrix44f&);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                  const gmtl::Matrix33f&);

   template gmtl::Matrix44f& sub(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                 const gmtl::Matrix44f&);
   template gmtl::Matrix33f& sub(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                 const gmtl::Matrix33f&);

   template gmtl::Matrix44f& add(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                 const gmtl::Matrix44f&);
   template gmtl::Matrix33f& add(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                 const gmtl::Matrix33f&);

   template gmtl::Matrix44f& postMult(gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template gmtl::Matrix33f& postMult(gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template gmtl::Matrix44f& preMult(gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template gmtl::Matrix33f& preMult(gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template gmtl::Matrix44f& mult(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                  const float&);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                  const float&);

   template gmtl::Matrix44f& mult(gmtl::Matrix44f&, float);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, float);

   template gmtl::Matrix44f& transpose(gmtl::Matrix44f&);
   template gmtl::Matrix33f& transpose(gmtl::Matrix33f&);

   template gmtl::Matrix44f& transpose(gmtl::Matrix44f&,
                                       const gmtl::Matrix44f&);
   template gmtl::Matrix33f& transpose(gmtl::Matrix33f&,
                                       const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invertTrans(gmtl::Matrix44f&,
                                         const gmtl::Matrix44f&);
   template gmtl::Matrix33f& invertTrans(gmtl::Matrix33f&,
                                         const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invertOrthogonal(gmtl::Matrix44f&,
                                              const gmtl::Matrix44f&);
   template gmtl::Matrix33f& invertOrthogonal(gmtl::Matrix33f&,
                                              const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invertAffine(gmtl::Matrix44f&,
                                          const gmtl::Matrix44f&);
   template gmtl::Matrix33f& invertAffine(gmtl::Matrix33f&,
                                          const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invertFull_GJ(gmtl::Matrix44f&,
                                           const gmtl::Matrix44f&);
   template gmtl::Matrix33f& invertFull_GJ(gmtl::Matrix33f&,
                                           const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invertFull(gmtl::Matrix44f&,
                                        const gmtl::Matrix44f&);
   template gmtl::Matrix33f& invertFull(gmtl::Matrix33f&,
                                        const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invert(gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template gmtl::Matrix33f& invert(gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invert(gmtl::Matrix44f&);
   template gmtl::Matrix33f& invert(gmtl::Matrix33f&);

   template bool isEqual(const gmtl::Matrix44f&, const gmtl::Matrix44f&,
                         const float);
   template bool isEqual(const gmtl::Matrix33f&, const gmtl::Matrix33f&,
                         const float);
}
#endif


#endif /* _PYGMTL_MATRIX_OPS_H_ */
