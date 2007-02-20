/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: MatrixOpsTest.h,v $
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
#ifndef _GMTL_MATRIX_OPS_TEST_SUITE_H_
#define _GMTL_MATRIX_OPS_TEST_SUITE_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class MatrixOpsTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(MatrixOpsTest);

      CPPUNIT_TEST(testMatrixIdentity);
      CPPUNIT_TEST(testMatrixSetTrans);
      CPPUNIT_TEST(testMatrixTranspose);
      CPPUNIT_TEST(testMatrixAddSub);
      CPPUNIT_TEST(testMatrixMult);
      CPPUNIT_TEST(testMatrixScalarMult);
      CPPUNIT_TEST(testMatInvert);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testMatrixIdentity();
      void testMatrixSetTrans();
      void testMatrixTranspose();
      void testMatrixAddSub();
      void testMatrixMult();
      void testMatrixScalarMult();
      void testMatInvert();
   };

   /**
    * Metric tests.
    */
   class MatrixOpsMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(MatrixOpsMetricTest);

      CPPUNIT_TEST(testMatrixTimeIdentity44f);
      CPPUNIT_TEST(testTimingMakeTrans);
      CPPUNIT_TEST(testMatrixTimeTranspose44f);
      CPPUNIT_TEST(testMatrixTimeTranspose33d);
      CPPUNIT_TEST(testMatrixTimeMult44_mult);
      CPPUNIT_TEST(testMatrixTimeMult44_operatorStar);
      CPPUNIT_TEST(testMatrixTimeMult44f_operatorStarStar);
      CPPUNIT_TEST(testMatrixTimeMult44d_operatorStarStar);
      CPPUNIT_TEST(testMatrixTimeMult33f_operatorStarStar);
      CPPUNIT_TEST(testMatrixTimeMult33d_operatorStarStar);
      CPPUNIT_TEST(testMatrixTimeAdd44);
      CPPUNIT_TEST(testMatrixTimeSub44);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testMatrixTimeIdentity44f();
      void testTimingMakeTrans();
      void testMatrixTimeTranspose44f();
      void testMatrixTimeTranspose33d();
      void testMatrixTimeMult44_mult();
      void testMatrixTimeMult44_operatorStar();
      void testMatrixTimeMult44f_operatorStarStar();
      void testMatrixTimeMult44d_operatorStarStar();
      void testMatrixTimeMult33f_operatorStarStar();
      void testMatrixTimeMult33d_operatorStarStar();
      void testMatrixTimeAdd44();
      void testMatrixTimeSub44();
   };
}

#endif
