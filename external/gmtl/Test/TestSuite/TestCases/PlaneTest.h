/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: PlaneTest.h,v $
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
#ifndef _GMTL_PLANE_TEST_H_
#define _GMTL_PLANE_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include <gmtl/Plane.h>

namespace gmtlTest
{
   class PlaneTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(PlaneTest);

      CPPUNIT_TEST(testCreation);
      CPPUNIT_TEST(testThreePtCreation);
      CPPUNIT_TEST(testNormPtCreation);
      CPPUNIT_TEST(testNormOffsetCreation);
      CPPUNIT_TEST(testCopyConstruct);
      CPPUNIT_TEST(testEqualityCompare);
      CPPUNIT_TEST(testIsEqual);
      CPPUNIT_TEST(testDistance);
      CPPUNIT_TEST(testWhichSide);
      CPPUNIT_TEST(testFindNearestPt);
      CPPUNIT_TEST(testFindReflect);

      CPPUNIT_TEST_SUITE_END();

   public:
      void setUp();

      // ------------------------
      // CREATION TESTS
      // ------------------------
      void testCreation();
      void testThreePtCreation();
      void testNormPtCreation();
      void testNormOffsetCreation();
      void testCopyConstruct();

      // --------------------------
      // Comparison tests
      // --------------------------
      void testEqualityCompare();
      void testIsEqual();

      // --------------------------
      // Operations tests
      // --------------------------
      void testDistance();
      void testWhichSide();
      void testFindNearestPt();

	  void testFindReflect();

   protected:
      gmtl::Point<float, 3> origin;
      gmtl::Point<float, 3> x1_pt;
      gmtl::Point<float, 3> y1_pt;
      gmtl::Point<float, 3> z1_pt;
      gmtl::Vec<float, 3> x1_v;
      gmtl::Vec<float, 3> y1_v;
      gmtl::Vec<float, 3> z1_v;

      gmtl::Plane<float> xy_plane;
      gmtl::Plane<float> zx_plane;
      gmtl::Plane<float> yz_plane;
   };
}

#endif
