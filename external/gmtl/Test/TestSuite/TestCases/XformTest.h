/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: XformTest.h,v $
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
#ifndef _GMTL_XFORM_TEST_H_
#define _GMTL_XFORM_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class XformTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(XformTest);

      CPPUNIT_TEST(testQuatVecXform);
      CPPUNIT_TEST(weird_XformQuatVec_InvConj_SanityCheck);
      CPPUNIT_TEST(testMatVecXform);
      CPPUNIT_TEST(testMatPointXform);
      CPPUNIT_TEST(testMatRayXform);
      CPPUNIT_TEST(testMatLineSegXform);
      
      CPPUNIT_TEST_SUITE_END();

   public:
      void testQuatVecXform();
      void weird_XformQuatVec_InvConj_SanityCheck();
      void testMatVecXform();
      void testMatPointXform();
      void testMatLineSegXform();
      void testMatRayXform();
   };

   /**
    * Metric tests.
    */
   class XformMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(XformMetricTest);

      CPPUNIT_TEST(testTimingXformQuatVec3);
      CPPUNIT_TEST(testTimingXformMatVecComplete);
      CPPUNIT_TEST(testTimingXformMatVecPartial);
      CPPUNIT_TEST(testTimingXformMatPointComplete);
      CPPUNIT_TEST(testTimingXformMatPointPartial);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testTimingXformQuatVec3();
      void testTimingXformMatVecComplete();
      void testTimingXformMatVecPartial();
      void testTimingXformMatPointComplete();
      void testTimingXformMatPointPartial();
   };
}

#endif
