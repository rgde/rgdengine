/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: MatrixGenTest.h,v $
 * Date modified: $Date: 2006/07/19 14:14:01 $
 * Version:       $Revision: 1.3 $
 * -----------------------------------------------------------------
 *
 *********************************************************** ggt-head end */
/*************************************************************** ggt-cpr beg
*
* GGT: The Generic Graphics Toolkit
* Copyright (C) 2001, 2002 Allen Bierbaum
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
#ifndef _GMTL_MATRIX_GEN_TEST_H_
#define _GMTL_MATRIX_GEN_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class MatrixGenTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(MatrixGenTest);

      CPPUNIT_TEST(testMatrixsetViewing);
      CPPUNIT_TEST(testMatrixsetTrans);
      CPPUNIT_TEST(testMatrixsetAxes);
      CPPUNIT_TEST(testMatrixsetDirCos);
      CPPUNIT_TEST(testMatrixsetScale);
      CPPUNIT_TEST(testMatrixsetRot);
      CPPUNIT_TEST(testMatrixsetRotEuler);
      CPPUNIT_TEST(testMatrixgetRotEuler);
      CPPUNIT_TEST(testMatrixGetSetRotEuler);
      CPPUNIT_TEST(testMatrixmakeInverse);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testMatrixsetViewing();
      void testMatrixsetTrans();
      void testMatrixsetAxes();
      void testMatrixsetDirCos();
      void testMatrixsetScale();
      void testMatrixsetRot();
      void testMatrixsetRotEuler();
      void testMatrixgetRotEuler();
      void testMatrixGetSetRotEuler();
      void testMatrixmakeInverse();
   };

   /**
    * Metric tests.
    */
   class MatrixGenMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(MatrixGenMetricTest);

      CPPUNIT_TEST(testTimingsetTrans);
      CPPUNIT_TEST(testTimingsetScale);
      CPPUNIT_TEST(testTimingsetRot33);
      CPPUNIT_TEST(testTimingsetRot34);
      CPPUNIT_TEST(testTimingsetRot44);
      CPPUNIT_TEST(testTimingGetRot);
      CPPUNIT_TEST(testTimingsetRotEuler33);
      CPPUNIT_TEST(testTimingsetRotEuler34);
      CPPUNIT_TEST(testTimingsetRotEuler44);
      CPPUNIT_TEST(testTimingsetDirCos33);
      CPPUNIT_TEST(testTimingsetDirCos34);
      CPPUNIT_TEST(testTimingsetDirCos44);
      CPPUNIT_TEST(testTimingsetAxes33);
      CPPUNIT_TEST(testTimingsetAxes34);
      CPPUNIT_TEST(testTimingsetAxes44);
      CPPUNIT_TEST(testTimingsetAxes);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testTimingsetTrans();
      void testTimingsetScale();
      void testTimingsetRot33();
      void testTimingsetRot34();
      void testTimingsetRot44();
      void testTimingGetRot();
      void testTimingsetRotEuler33();
      void testTimingsetRotEuler34();
      void testTimingsetRotEuler44();
      void testTimingsetDirCos33();
      void testTimingsetDirCos34();
      void testTimingsetDirCos44();
      void testTimingsetAxes33();
      void testTimingsetAxes34();
      void testTimingsetAxes44();
      void testTimingsetAxes();
   };
}

#endif
