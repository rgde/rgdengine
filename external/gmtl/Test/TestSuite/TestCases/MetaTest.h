/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: MetaTest.h,v $
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
#ifndef _GMTL_METATTEST_H_
#define _GMTL_METATTEST_H_

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <gmtl/Util/Meta.h>
#include <gmtl/Util/StaticAssert.h>

namespace gmtlTest
{
   class Type1
   {
   public:
      enum { VALUE = 1 };
   };
   class Type2
   {
   public:
      enum { VALUE = 1 };      
   };

   template <typename TYPE1, typename TYPE2>
   class AssertTestClass
   {
   public:      
      enum
      {
          Value1 = TYPE1::VALUE,
          Value2 = TYPE2::VALUE
      };
            
      void TestFunc( )
      {
         GMTL_STATIC_ASSERT(Value1 == 1);       // Incorrect size
         GMTL_STATIC_ASSERT(Value2 == 1);      
      }
   };

   
   /** Class to use for testing of Meta functions and all that jazz
   *
   * Note: Some tests in here do NOT compile (and are not supposed to)
   *       so they are commented out by default and are uncommented
   *       to actually test the stuff
   */
   class MetaTest : public CppUnit::TestCase
   {
   public:
      enum
      { ValueOne = 1, ValueTwo = 2, ValueThree = 1 };

      MetaTest( std::string name = "MetaTest" )
         : CppUnit::TestCase( name )
      {}

      virtual ~MetaTest()
      {}

      virtual void setUp()
      {}

      virtual void tearDown()
      {}

      void testStaticAssert()
      {
         GMTL_STATIC_ASSERT(1==1);
         GMTL_STATIC_ASSERT(2>1);
         GMTL_STATIC_ASSERT(ValueOne == ValueThree);
         GMTL_STATIC_ASSERT((1+1)==2);
         GMTL_STATIC_ASSERT(true);

         //GMTL_STATIC_ASSERT(ValueOne == ValueTwo);    /** Should fail */

         AssertTestClass<Type1, Type2> test_class;
         test_class.TestFunc();

      }

      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite( "OutputTest" );
         test_suite->addTest( new CppUnit::TestCaller<MetaTest>("testStaticAssert", &MetaTest::testStaticAssert) );
         
         return test_suite;
      }
            
   };
}

#endif
