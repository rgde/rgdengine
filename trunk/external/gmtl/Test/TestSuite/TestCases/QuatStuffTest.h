/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: QuatStuffTest.h,v $
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
#ifndef _GMTL_QUAT_STUFF_TEST_H_
#define _GMTL_QUAT_STUFF_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /** a hodge podge of tests.  stuff that is a little higher level, some real world tests, etc... */
   class QuatStuffTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(QuatStuffTest);

      CPPUNIT_TEST(makeRotTest);
      CPPUNIT_TEST(xformVecSweepTest);
      CPPUNIT_TEST(makeRotGetRotSanityTest);
      CPPUNIT_TEST(simpleQuatProductTest);
      CPPUNIT_TEST(secondQuatProductTest);
      CPPUNIT_TEST(xformVecTest);
      CPPUNIT_TEST(specialCases);
      CPPUNIT_TEST(vectorScaleTest);
      CPPUNIT_TEST(quatAdd);
      CPPUNIT_TEST(pureQuatMultTest);

      CPPUNIT_TEST_SUITE_END();

   public:
      void makeRotTest();
      void xformVecSweepTest();
      void makeRotGetRotSanityTest();

      void simpleQuatProductTest();

      /// another quaternion product test
      void secondQuatProductTest();
      void xformVecTest();

      /// just trying stuff out... TODO think of a good add test...
      void specialCases();

      /**
       * just trying stuff out... TODO think of a good way to test this ability...
       * TODO: could we use a vector (pure quat) to scale a quat in a
       *        more robust way?  pretty sure we can... rewrite scaleAngle to use this
       *        method instead....
       */
      void vectorScaleTest();

      /// just trying stuff out... TODO think of a good add test...
      void quatAdd();

      /**
       * using a vector to increment rotation, much like angular velocity
       * here angular velocity is defined as a gmtl::Vec3f "w", using right hand rule
       * things spin at |w| speed and on the w vector.
       *
       * just trying stuff out... TODO think of a good way to test this ability...
       */
      void pureQuatMultTest();
   };
}

#endif
