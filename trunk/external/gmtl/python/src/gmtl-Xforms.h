/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: gmtl-Xforms.h,v $
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

#ifndef _PYGMTL_XFORMS_H_
#define _PYGMTL_XFORMS_H_

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.

#include <gmtl/Xforms.h>


#if ! defined(__APPLE__)
namespace gmtl
{
   template gmtl::VecBase<float, 3>& xform(gmtl::VecBase<float, 3>&,
                                           const gmtl::Quat<float>&,
                                           const gmtl::VecBase<float, 3>&);
   template gmtl::VecBase<double, 3>& xform(gmtl::VecBase<double, 3>&,
                                            const gmtl::Quat<double>&,
                                            const gmtl::VecBase<double, 3>&);

   // XXX: Could probably add a gmtl::Matrix43[fd] here if that type were
   // exposed to Python as well.
   template gmtl::Vec<float, 3>& xform(gmtl::Vec<float, 3>&,
                                       const gmtl::Matrix<float, 3, 3>&,
                                       const gmtl::Vec<float, 3>&);
   template gmtl::Vec<double, 3>& xform(gmtl::Vec<double, 3>&,
                                        const gmtl::Matrix<double, 3, 3>&,
                                        const gmtl::Vec<double, 3>&);
   template gmtl::Vec<float, 4>& xform(gmtl::Vec<float, 4>&,
                                       const gmtl::Matrix<float, 4, 4>&,
                                       const gmtl::Vec<float, 4>&);
   template gmtl::Vec<double, 4>& xform(gmtl::Vec<double, 4>&,
                                        const gmtl::Matrix<double, 4, 4>&,
                                        const gmtl::Vec<double, 4>&);

   template gmtl::Vec<float, 3>& xform(gmtl::Vec<float, 3>&,
                                       const gmtl::Matrix<float, 4, 4>&,
                                       const gmtl::Vec<float, 3>&);
   template gmtl::Vec<double, 3>& xform(gmtl::Vec<double, 3>&,
                                        const gmtl::Matrix<double, 4, 4>&,
                                        const gmtl::Vec<double, 3>&);

   // XXX: Could probably add a gmtl::Matrix43[fd] here if that type were
   // exposed to Python as well.
   template gmtl::Point<float, 3>& xform(gmtl::Point<float, 3>&,
                                         const gmtl::Matrix<float, 3, 3>&,
                                         const gmtl::Point<float, 3>&);
   template gmtl::Point<double, 3>& xform(gmtl::Point<double, 3>&,
                                          const gmtl::Matrix<double, 3, 3>&,
                                          const gmtl::Point<double, 3>&);
   template gmtl::Point<float, 4>& xform(gmtl::Point<float, 4>&,
                                         const gmtl::Matrix<float, 4, 4>&,
                                         const gmtl::Point<float, 4>&);
   template gmtl::Point<double, 4>& xform(gmtl::Point<double, 4>&,
                                          const gmtl::Matrix<double, 4, 4>&,
                                          const gmtl::Point<double, 4>&);

   template gmtl::Point<float, 3>& xform(gmtl::Point<float, 3>&,
                                         const gmtl::Matrix<float, 4, 4>&,
                                         const gmtl::Point<float, 3>&);
   template gmtl::Point<double, 3>& xform(gmtl::Point<double, 3>&,
                                          const gmtl::Matrix<double, 4, 4>&,
                                          const gmtl::Point<double, 3>&);
}
#endif


#endif /* _PYGMTL_XFORMS_H_ */
