/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: OOBoxTest.h,v $
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
#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <gmtl/OOBox.h>

namespace gmtlTest
{

class OOBoxTest : public CppUnit::TestFixture
{
public:
   OOBoxTest( std::string name = "OOBoxTest")
   : TestCase (name)
   {;}

   virtual ~OOBoxTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}

   void testBoxCreation()
   {
      gmtl::OOBox box1;
      gmtl::OOBox box2;

      box1.center().set(0.0, 1.0f, -2.0f);
      box1.axis(0) = gmtl::XUnitVec3;
      box1.axis(1) = gmtl::YUnitVec3;
      box1.axis(2) = gmtl::ZUnitVec3;

      box2 = box1;

      CPPUNIT_ASSERT(box2.center() == gmtl::Point3(0.0, 1.0f, -2.0f));
      CPPUNIT_ASSERT(box2 == box1);
      CPPUNIT_ASSERT(box1 == box2);
   }

   void testGetBoxVerts()
   {
      gmtl::OOBox box1;

      // Create box centered on origin
      // Aligned with major axes
      // with half lens 1,2,3
      box1.center().set(0.0, 0.0f, 0.0f);
      box1.axis(0) = gmtl::XUnitVec3;
      box1.axis(1) = gmtl::YUnitVec3;
      box1.axis(2) = gmtl::ZUnitVec3;
      box1.halfLen(0) = 1.0f;
      box1.halfLen(1) = 2.0f;
      box1.halfLen(2) = 3.0f;

      gmtl::Point3 verts[8];
      box1.getVerts(verts);

      CPPUNIT_ASSERT(verts[0] == gmtl::Point3(-1.0f,-2.0f,-3.0f));   // 000
      CPPUNIT_ASSERT(verts[1] == gmtl::Point3(1.0f,-2.0f,-3.0f));   // 100
      CPPUNIT_ASSERT(verts[2] == gmtl::Point3(1.0f,2.0f,-3.0f));   // 110
      CPPUNIT_ASSERT(verts[3] == gmtl::Point3(-1.0f,2.0f,-3.0f));   // 010

      CPPUNIT_ASSERT(verts[4] == gmtl::Point3(-1.0f,-2.0f,3.0f));   // 001
      CPPUNIT_ASSERT(verts[5] == gmtl::Point3(1.0f,-2.0f,3.0f));   // 101
      CPPUNIT_ASSERT(verts[6] == gmtl::Point3(1.0f,2.0f,3.0f));   // 111
      CPPUNIT_ASSERT(verts[7] == gmtl::Point3(-1.0f,2.0f,3.0f));   // 011
   }


   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("OOBoxTest");
      test_suite->addTest( new CppUnit::TestCaller<OOBoxTest>("testBoxCreation", &OOBoxTest::testBoxCreation));
      test_suite->addTest( new CppUnit::TestCaller<OOBoxTest>("testGetBoxVerts", &OOBoxTest::testGetBoxVerts));

      return test_suite;
   }

   static CppUnit::Test* perfSuite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("OOBoxTiming");
      return test_suite;
   }
   
   static CppUnit::Test* interactiveSuite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("OOBoxTestInteractive");
      //test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
