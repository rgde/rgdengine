/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: QuatGenTest.h,v $
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
#ifndef _GMTL_QUAT_GEN_TEST_H_
#define _GMTL_QUAT_GEN_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

/// @todo test/timing on makeRot( quat ) (euler version)

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class QuatGenTest : public CppUnit::TestCase
   {
      CPPUNIT_TEST_SUITE(QuatGenTest);

      CPPUNIT_TEST(testQuatMakePure);
      CPPUNIT_TEST(testQuatMakeConj);
      CPPUNIT_TEST(testQuatMakeInvert);
      CPPUNIT_TEST(testQuatMakeRot);
      CPPUNIT_TEST(testQuatGetRot);
      CPPUNIT_TEST(testQuatMakeGetMakeRot);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testQuatMakePure();
      void testQuatMakeConj();
      void testQuatMakeInvert();
      void testQuatMakeRot();
      void testQuatGetRot();
      void testQuatMakeGetMakeRot();
   };

   /**
    * Metric tests.
    */
   class QuatGenMetricTest : public CppUnit::TestCase
   {
      CPPUNIT_TEST_SUITE(QuatGenMetricTest);

      CPPUNIT_TEST(testGenTimingMakeInvert1);
      CPPUNIT_TEST(testGenTimingMakeInvert2);
      CPPUNIT_TEST(testGenTimingMakeConj);
      CPPUNIT_TEST(testGenTimingMakePure);
      CPPUNIT_TEST(testGenTimingMakeNormalQuat);
      CPPUNIT_TEST(testGenTimingMakeRot);
      CPPUNIT_TEST(testGenTimingSetRot);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testGenTimingMakeInvert1();
      void testGenTimingMakeInvert2();
      void testGenTimingMakeConj();
      void testGenTimingMakePure();
      void testGenTimingMakeNormalQuat();
      void testGenTimingMakeRot();
      void testGenTimingSetRot();
   };
}

#endif
