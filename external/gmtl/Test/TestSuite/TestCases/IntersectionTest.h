/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: IntersectionTest.h,v $
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
#ifndef _GMTL_INTERSECTION_TEST_H_
#define _GMTL_INTERSECTION_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class IntersectionTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(IntersectionTest);

      CPPUNIT_TEST(testIntersectAABoxAABox);
      CPPUNIT_TEST(testIntersectAABoxPoint);
      CPPUNIT_TEST(testIntersectAABoxLineSeg);
      CPPUNIT_TEST(testIntersectAABoxRay);
      CPPUNIT_TEST(testIntersectAABoxSphere);

      CPPUNIT_TEST(testIntersectAABoxSweep);
      CPPUNIT_TEST(testIntersectSphereSweep);

	  CPPUNIT_TEST(testIntersectRayPlane);
	  CPPUNIT_TEST(testIntersectLineSegPlane);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testIntersectAABoxAABox();
      void testIntersectAABoxPoint();
      void testIntersectAABoxLineSeg();
      void testIntersectAABoxRay();
      void testIntersectAABoxSphere();

      void testIntersectAABoxSweep();
      void testIntersectSphereSweep();

	  void testIntersectRayPlane();
	  void testIntersectLineSegPlane();
   };

   /**
    * Metric tests.
    */
   class IntersectionMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(IntersectionMetricTest);

      CPPUNIT_TEST(testTimingIntersectAABoxAABox);
      CPPUNIT_TEST(testTimingIntersectAABoxPoint);

      CPPUNIT_TEST(testTimingIntersectAABoxSweep);
      CPPUNIT_TEST(testTimingIntersectSphereSweep);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testTimingIntersectAABoxAABox();
      void testTimingIntersectAABoxPoint();

      void testTimingIntersectAABoxSweep();
      void testTimingIntersectSphereSweep();
   };
}

#endif
