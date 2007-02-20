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
 * File:          $RCSfile: gmtl-getData-wrappers.h,v $
 * Date modified: $Date: 2006/07/19 14:15:55 $
 * Version:       $Revision: 1.3 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _PYJUGGLER_GMTL_GETDATA_WRAPPERS_H_
#define _PYJUGGLER_GMTL_GETDATA_WRAPPERS_H_

#include <boost/python/tuple.hpp>
#include <gmtl/Matrix.h>
#include <gmtl/VecBase.h>
#include <gmtl/Quat.h>

namespace gmtlWrappers
{
   template<typename T, unsigned SIZE>
   static boost::python::list makeDataList(T* data)
   {
      boost::python::list l;

      for ( unsigned int i = 0; i < SIZE; ++i )
      {
         l.append(data[i]);
      }

      return l;
   }

   template<typename T>
   boost::python::list Matrix_3_3_getData(gmtl::Matrix<T, 3, 3>* m)
   {
      return makeDataList<float, 9>(m->mData);
   }

   template<typename T>
   boost::python::list Matrix_4_4_getData(gmtl::Matrix<T, 4, 4>* m)
   {
      return makeDataList<float, 16>(m->mData);
   }

   template<typename T>
   boost::python::tuple VecBase_2_getData(gmtl::VecBase<T, 2>* v)
   {
      return boost::python::make_tuple(v->mData[0], v->mData[1]);
   }

   template<typename T>
   boost::python::tuple VecBase_3_getData(gmtl::VecBase<T, 3>* v)
   {
      return boost::python::make_tuple(v->mData[0], v->mData[1], v->mData[2]);
   }

   template<typename T>
   boost::python::tuple VecBase_4_getData(gmtl::VecBase<T, 4>* v)
   {
      return boost::python::make_tuple(v->mData[0], v->mData[1], v->mData[2],
                                       v->mData[3]);
   }

   template<typename T>
   boost::python::tuple Quat_getData(gmtl::Quat<T>* q)
   {
      return boost::python::make_tuple(q->mData[0], q->mData[1], q->mData[2],
                                       q->mData[3]);
   }

   template<typename TYPE, typename ROT>
   boost::python::tuple EulerAngle_getData(gmtl::EulerAngle<TYPE, ROT>* ang)
   {
      return boost::python::make_tuple(ang->getData()[0], ang->getData()[1],
                                       ang->getData()[2]);
   }
}

#if ! defined(__APPLE__)
namespace gmtlWrappers
{
   template boost::python::list Matrix_3_3_getData(gmtl::Matrix<float, 3, 3>*);

   template boost::python::list Matrix_4_4_getData(gmtl::Matrix<float, 4, 4>*);

   template boost::python::tuple VecBase_2_getData(gmtl::VecBase<double, 2>*);
   template boost::python::tuple VecBase_2_getData(gmtl::VecBase<float, 2>*);
   template boost::python::tuple VecBase_2_getData(gmtl::VecBase<int, 2>*);

   template boost::python::tuple VecBase_3_getData(gmtl::VecBase<double, 3>*);
   template boost::python::tuple VecBase_3_getData(gmtl::VecBase<float, 3>*);
   template boost::python::tuple VecBase_3_getData(gmtl::VecBase<int, 3>*);

   template boost::python::tuple VecBase_4_getData(gmtl::VecBase<double, 4>*);
   template boost::python::tuple VecBase_4_getData(gmtl::VecBase<float, 4>*);
   template boost::python::tuple VecBase_4_getData(gmtl::VecBase<int, 4>*);

   template boost::python::tuple Quat_getData(gmtl::Quatd*);
   template boost::python::tuple Quat_getData(gmtl::Quatf*);

   template boost::python::tuple EulerAngle_getData(gmtl::EulerAngleXYZf*);
   template boost::python::tuple EulerAngle_getData(gmtl::EulerAngleZYXf*);
   template boost::python::tuple EulerAngle_getData(gmtl::EulerAngleZXYf*);
}
#endif


#endif /* _GMTL_GETDATA_WRAPPERS_H_ */
