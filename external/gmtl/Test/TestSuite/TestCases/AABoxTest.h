/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: AABoxTest.h,v $
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
#ifndef _GMTL_AABOX_TEST_H_
#define _GMTL_AABOX_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class AABoxTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(AABoxTest);

      CPPUNIT_TEST(testCreation);
      CPPUNIT_TEST(testCopyConstruct);
      CPPUNIT_TEST(testConstructors);
      CPPUNIT_TEST(testGetMin);
      CPPUNIT_TEST(testGetMax);
      CPPUNIT_TEST(testIsEmpty);
      CPPUNIT_TEST(testSetMin);
      CPPUNIT_TEST(testSetMax);
      CPPUNIT_TEST(testSetEmpty);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testCreation();
      void testCopyConstruct();
      void testConstructors();

      // -- Test accessors --//
      void testGetMin();
      void testGetMax();
      void testIsEmpty();

      // -- Test setters --//
      void testSetMin();
      void testSetMax();
      void testSetEmpty();
   };

   /**
    * Metric tests.
    */
   class AABoxMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(AABoxMetricTest);

      CPPUNIT_TEST(testTimingCreation);
      CPPUNIT_TEST(testTimingCopyConstruct);
      CPPUNIT_TEST(testTimingConstructors);
      CPPUNIT_TEST(testTimingGetMin);
      CPPUNIT_TEST(testTimingGetMax);
      CPPUNIT_TEST(testTimingIsEmpty);
      CPPUNIT_TEST(testTimingSetMin);
      CPPUNIT_TEST(testTimingSetMax);
      CPPUNIT_TEST(testTimingSetEmpty);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testTimingCreation();
      void testTimingCopyConstruct();
      void testTimingConstructors();

      // -- Test accessors --//
      void testTimingGetMin();
      void testTimingGetMax();
      void testTimingIsEmpty();

      // -- Test setters --//
      void testTimingSetMin();
      void testTimingSetMax();
      void testTimingSetEmpty();
   };
}

#endif
