/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: PointTest.h,v $
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
#ifndef _GMTL_POINT_TEST_H_
#define _GMTL_POINT_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class PointTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(PointTest);

      CPPUNIT_TEST(testCreation);
      CPPUNIT_TEST(testCopyConstruct);
      CPPUNIT_TEST(testConstructors);
      CPPUNIT_TEST(testSet);
      CPPUNIT_TEST(testSetPtr);
      CPPUNIT_TEST(testGetData);
      CPPUNIT_TEST(testEqualityCompare);
      CPPUNIT_TEST(testIsEqual);
      CPPUNIT_TEST(testOpPlusEq);
      CPPUNIT_TEST(testOpPlus);
      CPPUNIT_TEST(testOpMinusEq);
      CPPUNIT_TEST(testOpMinus);
      CPPUNIT_TEST(testOpMultScalarEq);
      CPPUNIT_TEST(testOpMultScalar);
      CPPUNIT_TEST(testOpDivScalarEq);
      CPPUNIT_TEST(testOpDivScalar);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testCreation();
      void testCopyConstruct();
      void testConstructors();

      void testSet();
      void testSetPtr();
      void testGetData();

      // -- Test comparison -- //
      void testEqualityCompare();
      void testIsEqual();

      // ---------------------------------- //
      // ---------- Math ops -------------- //
      // ---------------------------------- //
      void testOpPlusEq();
      void testOpPlus();
      void testOpMinusEq();
      void testOpMinus();
      void testOpMultScalarEq();
      void testOpMultScalar();
      void testOpDivScalarEq();
      void testOpDivScalar();
   };


   /**
    * Metric tests.
    */
   class PointMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(PointMetricTest);

      CPPUNIT_TEST(testTimingCreation);
      CPPUNIT_TEST(testTimingCopyConstruct);
      CPPUNIT_TEST(testTimingConstructors);
      CPPUNIT_TEST(testTimingSet);
      CPPUNIT_TEST(testTimingSetPtr);
      CPPUNIT_TEST(testTimingEqualityCompare);
      CPPUNIT_TEST(testTimingIsEqual);
      CPPUNIT_TEST(testTimingOpPlusEq);
      CPPUNIT_TEST(testTimingOpPlus);
      CPPUNIT_TEST(testTimingOpMinusEq);
      CPPUNIT_TEST(testTimingOpMinus);
      CPPUNIT_TEST(testTimingOpMultScalarEq);
      CPPUNIT_TEST(testTimingOpMultScalar);
      CPPUNIT_TEST(testTimingOpDivScalarEq);
      CPPUNIT_TEST(testTimingOpDivScalar);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testTimingCreation();
      void testTimingCopyConstruct();
      void testTimingConstructors();

      void testTimingSet();
      void testTimingSetPtr();
      void testTimingGetData();

      void testTimingEqualityCompare();
      void testTimingIsEqual();

      void testTimingOpPlusEq();
      void testTimingOpPlus();
      void testTimingOpMinusEq();
      void testTimingOpMinus();
      void testTimingOpMultScalarEq();
      void testTimingOpMultScalar();
      void testTimingOpDivScalarEq();
      void testTimingOpDivScalar();
   };
}

#endif
