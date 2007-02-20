/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: MatrixClassTest.h,v $
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
#ifndef _GMTL_MATRIX_CLASS_TEST_H_
#define _GMTL_MATRIX_CLASS_TEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   /**
    * Functionality tests
    */
   class MatrixClassTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(MatrixClassTest);

      CPPUNIT_TEST(testMatrixIdentity);
      CPPUNIT_TEST(testMatrixSetPtr);
      CPPUNIT_TEST(testMatrixSetTransposePtr);
      CPPUNIT_TEST(testMatrix44Creation);
      CPPUNIT_TEST(testMatrix34Creation);
      CPPUNIT_TEST(testMatrix33Creation);
      CPPUNIT_TEST(testMatrix23Creation);
      CPPUNIT_TEST(testMatrix22Creation);

      CPPUNIT_TEST_SUITE_END();

   public:
      // test out operations using identity matrix
      void testMatrixIdentity();

      // make sure set( float* ) works
      void testMatrixSetPtr();

      // make sure set( float* ) works
      void testMatrixSetTransposePtr();

      // more general creation tests..
      // tests default constructor, copy constructor, set()
      //       operator() operator[]
      void testMatrix44Creation();
      void testMatrix34Creation();
      void testMatrix33Creation();
      void testMatrix23Creation();
      void testMatrix22Creation();
   };

   /**
    * Metric tests.
    */
   class MatrixClassMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(MatrixClassMetricTest);

      CPPUNIT_TEST(testTimingDefaultConstructor);
      CPPUNIT_TEST(testTimingCopyConstructor);
      CPPUNIT_TEST(testTimingOpEqual);
      CPPUNIT_TEST(testTimingOpParen);
      CPPUNIT_TEST(testTimingOpBracket);
      CPPUNIT_TEST(testTimingSetPtr);
      CPPUNIT_TEST(testTimingSetTransposePtr);
      CPPUNIT_TEST(testTimingGetData);
      CPPUNIT_TEST(testTimingSet);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testTimingDefaultConstructor();
      void testTimingCopyConstructor();

      void testTimingOpEqual();
      void testTimingOpParen();
      void testTimingOpBracket();

      void testTimingSetPtr();
      void testTimingSetTransposePtr();
      void testTimingGetData();
      void testTimingSet();
   };
}

#endif
