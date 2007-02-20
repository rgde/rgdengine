/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: QuatTest.h,v $
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

#include <gmtl/Quat.h>
#include <gmtl/Matrix.h>

namespace gmtlTest
{

class QuatTest : public CppUnit::TestFixture
{
public:
   QuatTest( std::string name = "QuatTest" )
   : TestCase (name)
   {;}

   virtual ~QuatTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testQuatTestCreation()
   {
      gmtl::Quat test_quat;
      test_quat.makeAxisAngle(50,Vec3(0,0,1));
   }

   void testGetSetAxes()
   {
      gmtl::Matrix mat;
      gmtl::Vec3 xAxis1,yAxis1,zAxis1;
      gmtl::Vec3 xAxis2,yAxis2,zAxis2;
      gmtl::Quat quat0;

      // Simple rotation around X axis
      mat.makeRot(90,gmtl::XUnitVec3);
      mat.getAxes(xAxis1,yAxis1,zAxis1);
      quat0.makeAxes(xAxis1,yAxis1,zAxis1);
      quat0.getAxes(xAxis2,yAxis2,zAxis2);
      CPPUNIT_ASSERT(xAxis1.equal(xAxis2,0.01));
      CPPUNIT_ASSERT(yAxis1.equal(yAxis2,0.01));
      CPPUNIT_ASSERT(zAxis1.equal(zAxis2,0.01));

      // More complex - Just get some orthonormal axis
      ///*
      xAxis1.set(7, 11, 21);     xAxis1.normalize();
      yAxis1.set(12, 21, 75);    yAxis1.normalize();
      zAxis1 = xAxis1.cross(yAxis1);   zAxis1.normalize();
      //*/
      /*
      mat.makeXYZEuler(45.0f, -35.0f, 13.0f);
      mat.getAxes(xAxis1,yAxis1,zAxis1);
      */

      // Test with Matrix
      /*
      mat.makeAxes(xAxis1,yAxis1,zAxis1);
      mat.getAxes(xAxis2,yAxis2,zAxis2);

      CPPUNIT_ASSERT(xAxis1.equal(xAxis2,0.01));
      CPPUNIT_ASSERT(yAxis1.equal(yAxis2,0.01));
      CPPUNIT_ASSERT(zAxis1.equal(zAxis2,0.01));

      // Test with quat
      quat0.makeAxes(xAxis1,yAxis1,zAxis1);
      quat0.getAxes(xAxis2,yAxis2,zAxis2);

      CPPUNIT_ASSERT(xAxis1.equal(xAxis2,0.01));
      CPPUNIT_ASSERT(yAxis1.equal(yAxis2,0.01));
      CPPUNIT_ASSERT(zAxis1.equal(zAxis2,0.01));
      */

   }

   


   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("QuatTest");
      test_suite->addTest( new CppUnit::TestCaller<QuatTest>("testMatrixCreate", &QuatTest::testQuatTestCreation));
      test_suite->addTest( new CppUnit::TestCaller<QuatTest>("testGetSetAxes", &QuatTest::testGetSetAxes));
      test_suite->addTest( new CppUnit::TestCaller<QuatTest>("testGetSetMat", &QuatTest::testGetSetMat));

      return test_suite;
   }

   static CppUnit::Test* perfSuite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("QuatTiming");
      return test_suite;
   }
   
   static CppUnit::Test* interactiveSuite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("InteractiveQuatTest");
      //test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
