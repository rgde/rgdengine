/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: SphereTest.h,v $
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
#ifndef _SPHERE_TEST_H
#define _SPHERE_TEST_H

#include <cppunit/extensions/HelperMacros.h>

namespace gmtlTest
{
   class SphereTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(SphereTest);

      CPPUNIT_TEST(testCreation);
      CPPUNIT_TEST(testCopyConstruct);
      CPPUNIT_TEST(testConstructors);
      CPPUNIT_TEST(testGetCenter);
      CPPUNIT_TEST(testGetRadius);
      CPPUNIT_TEST(testEqualityCompare);
      CPPUNIT_TEST(testIsEqual);
      CPPUNIT_TEST(testIsInVolumePoint);
      CPPUNIT_TEST(testIsInVolumeSphere);
      CPPUNIT_TEST(testIsOnVolume);
      CPPUNIT_TEST(testExtendVolumePoint);
      CPPUNIT_TEST(testExtendVolumeSphere);
      CPPUNIT_TEST(testMakeVolumePoint);
      CPPUNIT_TEST(testSphereIntersections);
//      CPPUNIT_TEST(testMakeVolumeSphere);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testCreation();
      void testCopyConstruct();
      void testConstructors();

      // -- Test accessors --//
      void testGetCenter();
      void testGetRadius();

      // -- Test setters --//
      void testSetCenter();
      void testSetRadius();

      // -- Test comparison -- //
      void testEqualityCompare();
      void testIsEqual();

      // intersection tests
      static void testSphereIntersect( float x, float y, float z );
      void testSphereIntersections();

      //---------------------------------------------------------------------------
      // Containment tests
      //---------------------------------------------------------------------------
      void testIsInVolumePoint();
      void testIsInVolumeSphere();
      void testIsOnVolume();
      void testExtendVolumePoint();
      void testExtendVolumeSphere();
      void testMakeVolumePoint();
   //   void testMakeVolumeSphere();
   };

   class SphereMetricTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE(SphereMetricTest);

      CPPUNIT_TEST(testTimingCreation);
      CPPUNIT_TEST(testTimingCopyConstruct);
      CPPUNIT_TEST(testTimingConstructors);
      CPPUNIT_TEST(testTimingGetCenter);
      CPPUNIT_TEST(testTimingGetRadius);
      CPPUNIT_TEST(testTimingEqualityCompare);
      CPPUNIT_TEST(testTimingIsEqual);
      CPPUNIT_TEST(testTimingIsInVolumePoint);
      CPPUNIT_TEST(testTimingIsInVolumeSphere);
      CPPUNIT_TEST(testTimingIsOnVolume);
      CPPUNIT_TEST(testTimingExtendVolumePoint);
      CPPUNIT_TEST(testTimingExtendVolumeSphere);
      CPPUNIT_TEST(testTimingMakeVolumePoint);

      CPPUNIT_TEST_SUITE_END();

   public:
      void testTimingCreation();
      void testTimingCopyConstruct();
      void testTimingConstructors();

      // -- Test accessors --//
      void testTimingGetCenter();
      void testTimingGetRadius();

      // -- Test setters --//
      void testTimingSetCenter();
      void testTimingSetRadius();

      // -- Test comparison -- //
      void testTimingEqualityCompare();
      void testTimingIsEqual();

      //---------------------------------------------------------------------------
      // Containment testTimings
      //---------------------------------------------------------------------------
      void testTimingIsInVolumePoint();
      void testTimingIsInVolumeSphere();
      void testTimingIsOnVolume();
      void testTimingExtendVolumePoint();
      void testTimingExtendVolumeSphere();
      void testTimingMakeVolumePoint();
   };
}

#endif
