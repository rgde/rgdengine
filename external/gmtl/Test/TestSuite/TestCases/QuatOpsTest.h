/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: QuatOpsTest.h,v $
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
#ifndef _GMTL_QUAT_OPS_TEST_H_
#define _GMTL_QUAT_OPS_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class QuatOpsTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(QuatOpsTest);

      CPPUNIT_TEST(testQuatMult);
      CPPUNIT_TEST(testQuatDiv);
      CPPUNIT_TEST(testQuatVectorMult);
      CPPUNIT_TEST(testQuatVectorAdd);
      CPPUNIT_TEST(testQuatVectorSub);
      CPPUNIT_TEST(testQuatVectorDot);
      CPPUNIT_TEST(testQuatNorm);
      CPPUNIT_TEST(testQuatMag);
      CPPUNIT_TEST(testQuatNormalize);
      CPPUNIT_TEST(testQuatConj);
      CPPUNIT_TEST(testQuatNegate);
      CPPUNIT_TEST(testQuatInvert);
      CPPUNIT_TEST(testQuatSlerp);
      CPPUNIT_TEST(testQuatLerp);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testQuatMult();
      void testQuatDiv();
      void testQuatVectorMult();
      void testQuatVectorAdd();
      void testQuatVectorSub();
      void testQuatVectorDot();
      void testQuatNorm();
      void testQuatMag();
      void testQuatNormalize();
      void testQuatConj();
      void testQuatNegate();
      void testQuatInvert();
      void testQuatSlerp();
      void testQuatLerp();
   };

   /**
    * Metric tests.
    */
   class QuatOpsMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(QuatOpsMetricTest);

      CPPUNIT_TEST(testQuatTimingNegate);
      CPPUNIT_TEST(testQuatTimingOperatorMinus);
      CPPUNIT_TEST(testQuatTimingMult);
      CPPUNIT_TEST(testQuatTimingOperatorMult);
      CPPUNIT_TEST(testQuatTimingDiv);
      CPPUNIT_TEST(testQuatTimingLerp);
      CPPUNIT_TEST(testQuatTimingVectorMult);
      CPPUNIT_TEST(testQuatTimingVectorAdd);
      CPPUNIT_TEST(testQuatTimingVectorSub);
      CPPUNIT_TEST(testQuatTimingVectorDot);
      CPPUNIT_TEST(testQuatTimingNorm);
      CPPUNIT_TEST(testQuatTimingMag);
      CPPUNIT_TEST(testQuatTimingNormalize);
      CPPUNIT_TEST(testQuatTimingConj);
      CPPUNIT_TEST(testQuatTimingInvert);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testQuatTimingNegate();
      void testQuatTimingOperatorMinus();
      void testQuatTimingMult();
      void testQuatTimingOperatorMult();
      void testQuatTimingDiv();
      void testQuatTimingLerp();
      void testQuatTimingVectorMult();
      void testQuatTimingVectorAdd();
      void testQuatTimingVectorSub();
      void testQuatTimingVectorDot();
      void testQuatTimingNorm();
      void testQuatTimingMag();
      void testQuatTimingNormalize();
      void testQuatTimingConj();
      void testQuatTimingInvert();
   };
}

#endif
