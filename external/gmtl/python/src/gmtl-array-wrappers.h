/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: gmtl-array-wrappers.h,v $
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

#ifndef _PYGMTL_GMTL_ARRAY_WRAPPERS_H_
#define _PYGMTL_GMTL_ARRAY_WRAPPERS_H_

#include <boost/python/list.hpp>
#include <gmtl/Matrix.h>
#include <gmtl/VecBase.h>


namespace gmtlWrappers
{
   template<typename T, unsigned int ROWS, unsigned int COLS>
   void Matrix_set(gmtl::Matrix<T, ROWS, COLS>* m, boost::python::list pyData)
   {
      T gmtl_data[ROWS * COLS];

      for ( unsigned int i = 0; i < ROWS * COLS; ++i )
      {
         gmtl_data[i] = boost::python::extract<T>(pyData[i]);
      }

      m->set(gmtl_data);
   }

   template<typename T, unsigned int ROWS, unsigned int COLS>
   void Matrix_setTranspose(gmtl::Matrix<T, ROWS, COLS>* m,
                            boost::python::list pyData)
   {
      T gmtl_data[ROWS * COLS];

      for ( unsigned int i = 0; i < ROWS * COLS; ++i )
      {
         gmtl_data[i] = boost::python::extract<T>(pyData[i]);
      }

      m->setTranspose(gmtl_data);
   }

   template<typename T, unsigned int SIZE>
   void VecBase_set(gmtl::VecBase<T, SIZE>* v, boost::python::list pyData)
   {
      T gmtl_data[SIZE];

      for ( unsigned int i = 0; i < SIZE; ++i )
      {
         gmtl_data[i] = boost::python::extract<T>(pyData[i]);
      }

      v->set(gmtl_data);
   }
}

#if ! defined(__APPLE__)
namespace gmtlWrappers
{
   template void Matrix_set(gmtl::Matrix<float, 3, 3>*, boost::python::list);
   template void Matrix_set(gmtl::Matrix<float, 4, 4>*, boost::python::list);

   template void Matrix_setTranspose(gmtl::Matrix<float, 3, 3>*,
                                     boost::python::list);
   template void Matrix_setTranspose(gmtl::Matrix<float, 4, 4>*,
                                     boost::python::list);

   template void VecBase_set(gmtl::VecBase<double, 2>*, boost::python::list);
   template void VecBase_set(gmtl::VecBase<double, 3>*, boost::python::list);
   template void VecBase_set(gmtl::VecBase<double, 4>*, boost::python::list);
   template void VecBase_set(gmtl::VecBase<float, 2>*, boost::python::list);
   template void VecBase_set(gmtl::VecBase<float, 3>*, boost::python::list);
   template void VecBase_set(gmtl::VecBase<float, 4>*, boost::python::list);
   template void VecBase_set(gmtl::VecBase<int, 2>*, boost::python::list);
   template void VecBase_set(gmtl::VecBase<int, 3>*, boost::python::list);
   template void VecBase_set(gmtl::VecBase<int, 4>*, boost::python::list);
}
#endif


#endif /* _PYGMTL_GMTL_ARRAY_WRAPPERS_H_ */
