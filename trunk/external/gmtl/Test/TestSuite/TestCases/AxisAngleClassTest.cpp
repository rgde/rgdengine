/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: AxisAngleClassTest.cpp,v $
 * Date modified: $Date: 2006/07/19 14:14:01 $
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
#include "AxisAngleClassTest.h"
#include "../Suites.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/MetricRegistry.h>
#include <gmtl/AxisAngle.h>

namespace gmtlTest
{
   CPPUNIT_TEST_SUITE_REGISTRATION(AxisAngleClassTest);
   CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AxisAngleClassMetricTest, Suites::metric());

   void AxisAngleClassTest::testAxisAngleClassTestCreation()
   {
      // test that it initializes to the identity
      gmtl::AxisAnglef q;
      CPPUNIT_ASSERT( q[0] == 0.0f );
      CPPUNIT_ASSERT( q[1] == 1.0f );
      CPPUNIT_ASSERT( q[2] == 0.0f );
      CPPUNIT_ASSERT( q[3] == 0.0f );

      // try out set...
      q.set( 1.0f, 2.0f, 3.0f, 902 );
      CPPUNIT_ASSERT( q[0] == 1.0f );
      CPPUNIT_ASSERT( q[1] == 2.0f );
      CPPUNIT_ASSERT( q[2] == 3.0f );
      CPPUNIT_ASSERT( q[3] == 902 );

      // try out setting with brackets
      q[0] = 5.0f;
      q[1] = 6.0f;
      q[2] = 7.0f;
      q[3] = 901;
      CPPUNIT_ASSERT( q[0] == 5.0f );
      CPPUNIT_ASSERT( q[1] == 6.0f );
      CPPUNIT_ASSERT( q[2] == 7.0f );
      CPPUNIT_ASSERT( q[3] == 901 );
     
      // try out element constructor
      gmtl::AxisAnglef q2( 10.0f, 11.0f, 12.0f, 902 );
      CPPUNIT_ASSERT( q2[0] == 10.0f );
      CPPUNIT_ASSERT( q2[1] == 11.0f );
      CPPUNIT_ASSERT( q2[2] == 12.0f );
      CPPUNIT_ASSERT( q2[3] == 902 );

      // try out copy constructor
      gmtl::AxisAnglef q3( q );
      CPPUNIT_ASSERT( q3[0] == 5.0f );
      CPPUNIT_ASSERT( q3[1] == 6.0f );
      CPPUNIT_ASSERT( q3[2] == 7.0f );
      CPPUNIT_ASSERT( q3[3] == 901 );

      // try out operator=() function
      gmtl::AxisAnglef q4;
      CPPUNIT_ASSERT( q4[0] == 0.0f );
      CPPUNIT_ASSERT( q4[1] == 1.0f );
      CPPUNIT_ASSERT( q4[2] == 0.0f );
      CPPUNIT_ASSERT( q4[3] == 0.0f );
      q4 = q2;
      CPPUNIT_ASSERT( q4[0] == 10.0f );
      CPPUNIT_ASSERT( q4[1] == 11.0f );
      CPPUNIT_ASSERT( q4[2] == 12.0f );
      CPPUNIT_ASSERT( q4[3] == 902 );

      // check out the const identities...
      gmtl::AxisAnglef q9( gmtl::AXISANGLE_IDENTITYF );
      CPPUNIT_ASSERT( q9[0] == 0.0f );
      CPPUNIT_ASSERT( q9[1] == 1.0f );
      CPPUNIT_ASSERT( q9[2] == 0.0f );
      CPPUNIT_ASSERT( q9[3] == 0.0f );
   }

   void AxisAngleClassMetricTest::testAxisAngleTimingDefaultConstructor()
   {
      const long iters( 400000 );
      float use_value(1);

      CPPUNIT_METRIC_START_TIMING();
      for (long iter = 0; iter < iters; ++iter)
      {
         // performance of def constructor
         gmtl::AxisAnglef q;
         use_value += q.mData[0];
      }
      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("AxisAngleTest/DefaultConstructor", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

      CPPUNIT_ASSERT( use_value != 0 );
   }

   void AxisAngleClassMetricTest::testAxisAngleTimingElementConstructor()
   {
      const long iters( 400000 );
      float use_value(1);

      CPPUNIT_METRIC_START_TIMING();
      for (long iter = 0; iter < iters; ++iter)
      {
         // performance of element constructor
         gmtl::AxisAnglef q2( 10.0f, 11.0f, 12.0f, 901 );
         use_value += q2.mData[0];
      }
      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("AxisAngleTest/ElementConstructor", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

      CPPUNIT_ASSERT( use_value != 0 );
   }

   void AxisAngleClassMetricTest::testAxisAngleTimingCopyConstructor()
   {
      const long iters( 400000 );
      float use_value(1);
      gmtl::AxisAnglef q( 67.0f, 68.0f, 69.0f, 901 );

      CPPUNIT_METRIC_START_TIMING();
      for (long iter = 0; iter < iters; ++iter)
      {
         // performance of copy constructor
         gmtl::AxisAnglef q3( q );
         use_value += q3.mData[0];
      }
      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("AxisAngleTest/CopyConstructor", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

      CPPUNIT_ASSERT( use_value != 0 );
   }

   void AxisAngleClassMetricTest::testAxisAngleTimingSet()
   {
      const long iters( 400000 );
      float use_value(0);
      CPPUNIT_METRIC_START_TIMING();
      gmtl::AxisAnglef q;
      for (long iter = 0; iter < iters; ++iter)
      {
         // performance of set...
         q.set( 1, 2, 3, 901 );
         use_value += q[0];
      }
      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("AxisAngleTest/set()", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

      CPPUNIT_ASSERT( use_value >= 0.0f );
   }

   void AxisAngleClassMetricTest::testAxisAngleTimingOpBracket()
   {
      const long iters( 400000 );
      float use_value(0);
      CPPUNIT_METRIC_START_TIMING();
      gmtl::AxisAnglef q;
      float x = 102.0f, y = 103.0f, z = 101.0f;
      float w = 901;
      for (long iter = 0; iter < iters; ++iter)
      {
         // performance of brackets
         q[0] = x;
         q[1] = y;
         q[2] = z;
         q[3] = w;
         use_value = use_value + x + y + z + w;
      }
      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("AxisAngleTest/operator[]()", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

      CPPUNIT_ASSERT( use_value > 0.0f );
   }

   void AxisAngleClassMetricTest::testAxisAngleTimingGetData()
   {
      const long iters( 400000 );
      float use_value(0);
      CPPUNIT_METRIC_START_TIMING();
      gmtl::AxisAnglef q( 1, 2, 3, 901 );
      for (long iter = 0; iter < iters; ++iter)
      {
         // performance of getData...
         const float* d = q.getData();
         use_value += d[1];
      }
      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("AxisAngleTest/getData()", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

      CPPUNIT_ASSERT( use_value > 0.0f );
   }

   void AxisAngleClassMetricTest::testAxisAngleTimingOpEqual()
   {
      const long iters( 400000 );
      gmtl::AxisAnglef q4, q2( 0.0f, 2.0f, 1.0f, 901 );
      CPPUNIT_METRIC_START_TIMING();
      for (long iter = 0; iter < iters; ++iter)
      {
         // performance of operator=() function
         q4 = q2;
         q2[0] += q4[2];
      }
      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("AxisAngleTest/operator=()", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

      CPPUNIT_ASSERT( q4[0] != 3498.0f );
   }
}
