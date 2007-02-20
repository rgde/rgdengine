/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: TriTest.h,v $
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
#ifndef _TRI_TEST_H_
#define _TRI_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functional tests.
    */
   class TriTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(TriTest);

      CPPUNIT_TEST(testCreation);
      CPPUNIT_TEST(testCopyConstruct);
      CPPUNIT_TEST(testConstructors);
      CPPUNIT_TEST(testVertexAccessor);
      CPPUNIT_TEST(testEdges);
      CPPUNIT_TEST(testEqualOps);
      CPPUNIT_TEST(testIsEqual);
      CPPUNIT_TEST(testCenter);
      CPPUNIT_TEST(testNormal);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testCreation();
      void testCopyConstruct();
      void testConstructors();

      //---------------------------------------------------------------------------
      // Begin testing accessors
      //---------------------------------------------------------------------------
      void testVertexAccessor();
      void testEdges();

      //---------------------------------------------------------------------------
      // Begin testing comparators
      //---------------------------------------------------------------------------
      void testEqualOps();
      void testIsEqual();
      void testCenter();
      void testNormal();
   };

   /**
    * Metric tests.
    */
   class TriMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(TriMetricTest);

      CPPUNIT_TEST(testTimingCreation);
      CPPUNIT_TEST(testTimingCopyConstruct);
      CPPUNIT_TEST(testTimingConstructors);
      CPPUNIT_TEST(testTimingVertexAccessor);
      CPPUNIT_TEST(testTimingEdges);
      CPPUNIT_TEST(testTimingEqualOps);
      CPPUNIT_TEST(testTimingIsEqual);
      CPPUNIT_TEST(testTimingCenter);
      CPPUNIT_TEST(testTimingNormal);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testTimingCreation();
      void testTimingCopyConstruct();
      void testTimingConstructors();
      void testTimingVertexAccessor();
      void testTimingEdges();
      void testTimingEqualOps();
      void testTimingIsEqual();
      void testTimingCenter();
      void testTimingNormal();
   };
}

#endif
