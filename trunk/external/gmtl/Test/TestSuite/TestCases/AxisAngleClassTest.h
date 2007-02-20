/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: AxisAngleClassTest.h,v $
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
#ifndef _AXIS_ANGLE_CLASS_TEST_
#define _AXIS_ANGLE_CLASS_TEST_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class AxisAngleClassTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(AxisAngleClassTest);

      CPPUNIT_TEST(testAxisAngleClassTestCreation);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testAxisAngleClassTestCreation();
   };

   /**
    * Metric tests.
    */
   class AxisAngleClassMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(AxisAngleClassMetricTest);

      CPPUNIT_TEST(testAxisAngleTimingDefaultConstructor);
      CPPUNIT_TEST(testAxisAngleTimingElementConstructor);
      CPPUNIT_TEST(testAxisAngleTimingCopyConstructor);
      CPPUNIT_TEST(testAxisAngleTimingSet);
      CPPUNIT_TEST(testAxisAngleTimingOpBracket);
      CPPUNIT_TEST(testAxisAngleTimingGetData);
      CPPUNIT_TEST(testAxisAngleTimingOpEqual);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testAxisAngleTimingDefaultConstructor();
      void testAxisAngleTimingElementConstructor();
      void testAxisAngleTimingCopyConstructor();
      void testAxisAngleTimingSet();
      void testAxisAngleTimingOpBracket();
      void testAxisAngleTimingGetData();
      void testAxisAngleTimingOpEqual();
   };
}

#endif
