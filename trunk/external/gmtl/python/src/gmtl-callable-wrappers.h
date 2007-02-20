/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: gmtl-callable-wrappers.h,v $
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

#ifndef _PYGMTL_CALLABLE_WRAPPERS_H_
#define _PYGMTL_CALLABLE_WRAPPERS_H_

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.

#include <gmtl/Matrix.h>


namespace gmtlWrappers
{
   template<typename DATA_TYPE, unsigned ROWS, unsigned COLS>
   DATA_TYPE callable(gmtl::Matrix<DATA_TYPE, ROWS, COLS>* m,
                      const unsigned int row, const unsigned int col)
   {
      return (*m)(row, col);
   }
}

#if ! defined(__APPLE__)
namespace gmtlWrappers
{
   template float callable(gmtl::Matrix33f*, const unsigned int,
                           const unsigned int);
   template float callable(gmtl::Matrix44f*, const unsigned int,
                           const unsigned int);
}
#endif


#endif /* _PYGMTL_CALLABLE_WRAPPERS_H_ */
