/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: LineSegTest.h,v $
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
#ifndef _GMTL_LINE_SEG_TEST_H_
#define _GMTL_LINE_SEG_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include <gmtl/LineSeg.h>
#include <gmtl/Plane.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class LineSegTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(LineSegTest);

      CPPUNIT_TEST(testCreation);
      CPPUNIT_TEST(testPtVecCreation);
      CPPUNIT_TEST(testPtPtCreation);
      CPPUNIT_TEST(testCopyConstruct);

      CPPUNIT_TEST(testGetOrigin);
      CPPUNIT_TEST(testSetOrigin);
      CPPUNIT_TEST(testGetDir);
      CPPUNIT_TEST(testSetDir);
      CPPUNIT_TEST(testGetLength);

      CPPUNIT_TEST(testEqualityCompare);
      CPPUNIT_TEST(testIsEqual);

      CPPUNIT_TEST(intersectLineSegPlane);
      CPPUNIT_TEST(intersectLineSegTri);
      CPPUNIT_TEST(intersectRayPlane);
      CPPUNIT_TEST(intersectRayTri);

//      CPPUNIT_TEST(testDistance);
//      CPPUNIT_TEST(testWhichSide);
//      CPPUNIT_TEST(testFindNearestPt);

      CPPUNIT_TEST_SUITE_END();

   public:
      void setUp();

      // ------------------------
      // CREATION TESTS
      // ------------------------
      void testCreation();
      void testPtVecCreation();
      void testPtPtCreation();
      void testCopyConstruct();

      void testGetOrigin();
      void testSetOrigin();
      void testGetDir();
      void testSetDir();
      void testGetLength();

      void testEqualityCompare();
      void testIsEqual();

      void intersectLineSegPlane();
      void intersectLineSegTri();
      void intersectRayPlane();
      void intersectRayTri();

//      void testDistance();
//      void testWhichSide();
//      void testFindNearestPt();

   protected:
      gmtl::Point<float, 3> origin;
      gmtl::Point<float, 3> x1_pt;
      gmtl::Point<float, 3> y1_pt;
      gmtl::Point<float, 3> z1_pt;
      gmtl::Vec<float, 3> x1_v;
      gmtl::Vec<float, 3> y1_v;
      gmtl::Vec<float, 3> z1_v;

      gmtl::LineSeg<float> x1_lineseg;
      gmtl::LineSeg<float> y1_lineseg;
      gmtl::LineSeg<float> z1_lineseg;
   };

   /**
    * Metric tests.
    */
   class LineSegMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(LineSegMetricTest);

      CPPUNIT_TEST(testTimingCreation);
      CPPUNIT_TEST(testTimingPtVecCreation);
      CPPUNIT_TEST(testTimingPtPtCreation);
      CPPUNIT_TEST(testTimingCopyConstruct);
      CPPUNIT_TEST(testTimingGetOrigin);
      CPPUNIT_TEST(testTimingSetOrigin);
      CPPUNIT_TEST(testTimingGetDir);
      CPPUNIT_TEST(testTimingSetDir);
      CPPUNIT_TEST(testTimingGetLength);
      CPPUNIT_TEST(testTimingEqualityCompare);
      CPPUNIT_TEST(testTimingIsEqual);

      CPPUNIT_TEST_SUITE_END();

   public:
      void setUp();

      void testTimingCreation();
      void testTimingPtVecCreation();
      void testTimingPtPtCreation();
      void testTimingCopyConstruct();
      void testTimingGetOrigin();
      void testTimingSetOrigin();
      void testTimingGetDir();
      void testTimingSetDir();
      void testTimingGetLength();
      void testTimingEqualityCompare();
      void testTimingIsEqual();

   protected:
      gmtl::Point<float, 3> origin;
      gmtl::Point<float, 3> x1_pt;
      gmtl::Point<float, 3> y1_pt;
      gmtl::Point<float, 3> z1_pt;
      gmtl::Vec<float, 3> x1_v;
      gmtl::Vec<float, 3> y1_v;
      gmtl::Vec<float, 3> z1_v;

      gmtl::LineSeg<float> x1_lineseg;
      gmtl::LineSeg<float> y1_lineseg;
      gmtl::LineSeg<float> z1_lineseg;
   };
}

#endif
