/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: Containment.h,v $
 * Date modified: $Date: 2006/07/19 14:12:43 $
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
#ifndef _GMTL_CONTAINMENT_H_
#define _GMTL_CONTAINMENT_H_

// new stuff
#include <vector>
#include <gmtl/Sphere.h>
#include <gmtl/AABox.h>
#include <gmtl/VecOps.h>

namespace math
{

//-----------------------------------------------------------------------------
// Sphere
//-----------------------------------------------------------------------------

/**
 * Tests if the given point is inside or on the surface of the given spherical
 * volume.
 *
 * @param container  the sphere to test against
 * @param pt         the point to test with
 *
 * @return  true if pt is inside container, false otherwise
 */
template< class DATA_TYPE >
bool isInVolume( const Sphere<DATA_TYPE>& container,
                 const Point<DATA_TYPE, 3>& pt )
{
   // The point is inside the sphere if the vector computed from the center of
   // the sphere to the point has a magnitude less than or equal to the radius
   // of the sphere.
   // |pt - center| <= radius
   return ( length(math::Vec<DATA_TYPE,3>(pt - container.mCenter)) <= container.mRadius );
}

/**
 * Tests if the given sphere is completely inside or on the surface of the given
 * spherical volume.
 *
 * @param container  the sphere acting as the container
 * @param sphere     the sphere that may be inside container
 *
 * @return  true if sphere is inside container, false otherwise
 */
template< class DATA_TYPE >
bool isInVolume( const Sphere<DATA_TYPE>& container,
                 const Sphere<DATA_TYPE>& sphere )
{
   // the sphere is inside container if the distance between the centers of the
   // spheres plus the radius of the inner sphere is less than or equal to the
   // radius of the containing sphere.
   // |sphere.center - container.center| + sphere.radius <= container.radius
   return ( length(math::Vec<DATA_TYPE,3>(sphere.mCenter - container.mCenter)) + sphere.mRadius
            <= container.mRadius );
}

/**
 * Modifies the existing sphere to tightly enclose itself and the given point.
 *
 * @param container  [in,out] the sphere that will be extended
 * @param pt         [in]     the point which the sphere should contain
 */
template< class DATA_TYPE >
void extendVolume( Sphere<DATA_TYPE>& container,
                   const Point<DATA_TYPE, 3>& pt )
{
   // check if we already contain the point
   if ( isInVolume( container, pt ) )
   {
      return;
   }

   // make a vector pointing from the center of the sphere to pt. this is the
   // direction in which we need to move the sphere's center
   Vec<DATA_TYPE, 3> dir = pt - container.mCenter;
   DATA_TYPE len = normalize( dir );

   // compute what the new radius should be
   DATA_TYPE newRadius =  (len + container.mRadius) * DATA_TYPE(0.5);

   // compute the new center for the sphere
   Point<DATA_TYPE, 3> newCenter = container.mCenter +
                                   (dir * (newRadius - container.mRadius));

   // modify container to its new values
   container.mCenter = newCenter;
   container.mRadius = newRadius;
}

/**
 * Modifies the container to tightly enclose itself and the given sphere.
 *
 * @param container  [in,out] the sphere that will be extended
 * @param sphere     [in]     the sphere which container should contain
 */
template< class DATA_TYPE >
void extendVolume( Sphere<DATA_TYPE>& container,
                   const Sphere<DATA_TYPE>& sphere )
{
   // check if we already contain the sphere
   if ( isInVolume( container, sphere ) )
   {
      return;
   }

   // make a vector pointing from the center of container to sphere. this is the
   // direction in which we need to move container's center
   Vec<DATA_TYPE, 3> dir = sphere.mCenter - container.mCenter;
   DATA_TYPE len = normalize( dir );

   // compute what the new radius should be
   DATA_TYPE newRadius = (len + sphere.mRadius + container.mRadius) *
                         DATA_TYPE(0.5);

   // compute the new center for container
   Point<DATA_TYPE, 3> newCenter = container.mCenter +
                                   (dir * (newRadius - container.mRadius));

   // modify container to its new values
   container.mCenter = newCenter;
   container.mRadius = newRadius;
}

/**
 * Modifies the given sphere to tightly enclose all points in the given
 * std::vector. This operation is O(n) and uses sqrt(..) liberally. :(
 *
 * @param container  [out]    the sphere that will be modified to tightly
 *                            enclose all the points in pts
 * @param pts        [in]     the list of points to contain
 *
 * @pre  pts must contain at least 2 points
 */
template< class DATA_TYPE >
void makeVolume( Sphere<DATA_TYPE>& container,
                 const std::vector< Point<DATA_TYPE, 3> >& pts )
{
   gmtlASSERT( pts.size() > 0  && "pts must contain at least 1 point" );

   // Implementation based on the Sphere Centered at Average of Points algorithm
   // found in "3D Game Engine Design" by Devud G, Eberly (pg. 27)
   typename std::vector< Point<DATA_TYPE, 3> >::const_iterator itr = pts.begin();

   // compute the average of the points as the center
   Point<DATA_TYPE, 3> sum = *itr;
   ++itr;
   while ( itr != pts.end() )
   {
      sum += *itr;
      ++itr;
   }
   container.mCenter = sum / DATA_TYPE(pts.size());

   // compute the distance from the computed center to point furthest from that
   // center as the radius
   DATA_TYPE radiusSqr(0);
   for ( itr = pts.begin(); itr != pts.end(); ++itr )
   {
      DATA_TYPE len = lengthSquared( math::Vec<DATA_TYPE,3>( (*itr) - container.mCenter) );
      if ( len > radiusSqr )
         radiusSqr = len;
   }

   container.mRadius = Math::sqrt( radiusSqr );
}

/*
template< class DATA_TYPE >
void makeVolume( Sphere<DATA_TYPE>& container,
                 const std::vector< Point<DATA_TYPE, 3> >& pts )
{
   gmtlASSERT( pts.size() > 1  && "pts must contain at least 2 points" );

   // make a sphere around the first 2 points and then extend the sphere by each
   // point thereafter. we could probably be smarter about this ...
   std::vector< Point<DATA_TYPE, 3> >::const_iterator itr = pts.begin();

   // make the initial sphere
   const Point<DATA_TYPE, 3>& first = *itr;
   ++itr;
   const Point<DATA_TYPE, 3>& second = *itr;
   ++itr;
   const Vec<DATA_TYPE, 3> dir = second - first;
   container.mRadius = length(dir) * DATA_TYPE(0.5);
   container.mCenter = first + (dir * container.mRadius);

   // iterate through the remaining points and extend the container to fit each
   // point. yay code reuse!
   while ( itr != pts.end() )
   {
      extendVolume( container, *itr );
      ++itr;
   }
}
*/
/**
 * Modifies the given sphere to tightly enclose all spheres in the given
 * std::vector. This operation is O(n) and uses sqrt(..) liberally. :(
 *
 * @param container  [out]    the sphere that will be modified to tightly
 *                            enclose all the spheres in spheres
 * @param spheres    [in]     the list of spheres to contain
 *
 * @pre  spheres must contain at least 2 points
 */
/*
template< class DATA_TYPE >
void makeVolume( Sphere<DATA_TYPE>& container,
                 const std::vector< Sphere<DATA_TYPE> >& spheres )
{
   gmtlASSERT( spheres.size() > 1  && "spheres must contain at least 2 points" );

   // make a sphere around the first 2 points and then extend the sphere by each
   // point thereafter. we could probably be smarter about this ...
   std::vector< Sphere<DATA_TYPE> >::const_iterator itr = spheres.begin();

   // make the initial sphere
   const Sphere<DATA_TYPE>& first = *itr;
   ++itr;
   const Sphere<DATA_TYPE>& second = *itr;
   ++itr;
   const Vec<DATA_TYPE, 3> dir = second.mCenter - first.mCenter;
   container.mRadius = (length(dir) + first.mRadius + second.mRadius) *
                       DATA_TYPE(0.5);
   container.mCenter = first.mCenter +
                       (dir * (container.mRadius - first.mRadius));

   // iterate through the remaining points and extend the container to fit each
   // point. yay code reuse!
   while ( itr != spheres.end() )
   {
      extendVolume( container, *itr );
      ++itr;
   }
}
*/
/**
 * Tests if the given point is on the surface of the container with zero
 * tolerance.
 *
 * @param container     the container to test against
 * @param pt            the test point
 *
 * @return  true if pt is on the surface of container, false otherwise
 */
template< class DATA_TYPE >
bool isOnVolume( const Sphere<DATA_TYPE>& container,
                 const Point<DATA_TYPE, 3>& pt )
{
   // |center - pt| - radius == 0
   return ( length(math::Vec<DATA_TYPE,3>(container.mCenter - pt)) - container.mRadius == 0 );
}

/**
 * Tests of the given point is on the surface of the container with the given
 * tolerance.
 *
 * @param container     the container to test against
 * @param pt            the test point
 * @param tol           the epsilon tolerance
 *
 * @return  true if pt is on the surface of container, false otherwise
 */
template< class DATA_TYPE >
bool isOnVolume( const Sphere<DATA_TYPE>& container,
                 const Point<DATA_TYPE, 3>& pt,
                 const DATA_TYPE& tol )
{
   gmtlASSERT( tol >= 0 && "tolerance must be positive" );

   // abs( |center-pt| - radius ) < tol
   return ( Math::abs( length( math::Vec<DATA_TYPE,3>(container.mCenter - pt)) - container.mRadius )
            <= tol );
}

//-----------------------------------------------------------------------------
// AABox
//-----------------------------------------------------------------------------

/**
 * Tests if the given point is inside (or on) the surface of the given AABox
 * volume.
 *
 * @param container  the AABox to test against
 * @param pt         the point to test with
 *
 * @return  true if pt is inside container, false otherwise
 */
template< class DATA_TYPE>
bool isInVolume(const AABox<DATA_TYPE>& container,
                const Point<DATA_TYPE, 3>& pt)
{
   if (! container.isEmpty())
   {
      return ( pt[0] >= container.mMin[0] &&
               pt[1] >= container.mMin[1] &&
               pt[2] >= container.mMin[2] &&
               pt[0] <= container.mMax[0] &&
               pt[1] <= container.mMax[1] &&
               pt[2] <= container.mMax[2]);
   }
   else
   {
      return false;
   }
}

/**
 * Tests if the given point is inside (not on) the surface of the given AABox
 * volume.  This method is "exclusive" because it does not consider the surface
 * to be a part of the space.
 *
 * @param container  the AABox to test against
 * @param pt         the point to test with
 *
 * @return  true if pt is inside container (but not on surface), false otherwise
 */
template< class DATA_TYPE>
bool isInVolumeExclusive(const AABox<DATA_TYPE>& container,
                const Point<DATA_TYPE, 3>& pt)
{
   if (! container.isEmpty())
   {
      return ( pt[0] > container.mMin[0] &&
               pt[1] > container.mMin[1] &&
               pt[2] > container.mMin[2] &&
               pt[0] < container.mMax[0] &&
               pt[1] < container.mMax[1] &&
               pt[2] < container.mMax[2]);
   }
   else
   {
      return false;
   }
}




/**
 * Tests if the given AABox is completely inside or on the surface of the given
 * AABox container.
 *
 * @param container  the AABox acting as the container
 * @param box        the AABox that may be inside container
 *
 * @return  true if AABox is inside container, false otherwise
 */
template< class DATA_TYPE >
bool isInVolume(const AABox<DATA_TYPE>& container,
                const AABox<DATA_TYPE>& box)
{
   // Empty boxes don't overlap
   if (container.isEmpty() || box.isEmpty())
   {
      return false;
   }

   // Test that the boxes are not overlapping on any axis
   if (container.mMax[0] < box.mMin[0] || container.mMin[0] > box.mMax[0] ||
       container.mMax[1] < box.mMin[1] || container.mMin[1] > box.mMax[1] ||
       container.mMax[2] < box.mMin[2] || container.mMin[2] > box.mMax[2])
   {
      return false;
   }
   else
   {
      return true;
   }
}

/**
 * Modifies the existing AABox to tightly enclose itself and the given point.
 *
 * @param container  [in,out] the AABox that will be extended
 * @param pt         [in]     the point which the AABox should contain
 */
template< class DATA_TYPE >
void extendVolume(AABox<DATA_TYPE>& container,
                  const Point<DATA_TYPE, 3>& pt)
{
   if (! container.isEmpty())
   {
      // X coord
      if (pt[0] > container.mMax[0])
      {
         container.mMax[0] = pt[0];
      }
      else if (pt[0] < container.mMin[0])
      {
         container.mMin[0] = pt[0];
      }

      // Y coord
      if (pt[1] > container.mMax[1])
      {
         container.mMax[1] = pt[1];
      }
      else if (pt[1] < container.mMin[1])
      {
         container.mMin[1] = pt[1];
      }

      // Z coord
      if (pt[2] > container.mMax[2])
      {
         container.mMax[2] = pt[2];
      }
      else if (pt[2] < container.mMin[2])
      {
         container.mMin[2] = pt[2];
      }
   }
   else
   {
      // Make a box with essentially zero volume at the point
      container.setMin(pt);
      container.setMax(pt);
      container.setEmpty(false);
   }
}

/**
 * Modifies the container to tightly enclose itself and the given AABox.
 *
 * @param container  [in,out] the AABox that will be extended
 * @param box        [in]     the AABox which container should contain
 */
template< class DATA_TYPE >
void extendVolume(AABox<DATA_TYPE>& container,
                  const AABox<DATA_TYPE>& box)
{
   // Can't extend by an empty box
   if (box.isEmpty())
   {
      return;
   }

   // An empty container is extended to be the box
   if (container.isEmpty())
   {
      container = box;
   }

   // Just extend by the corners of the box
   extendVolume(container, box.getMin());
   extendVolume(container, box.getMax());
}

/**
 * Creates an AABox that tightly encloses the given Sphere.
 *
 * @param box     set to the box
 */
template< class DATA_TYPE >
void makeVolume(AABox<DATA_TYPE>& box, const Sphere<DATA_TYPE>& sph)
{
   const math::Point<DATA_TYPE, 3>& center = sph.getCenter();
   const DATA_TYPE& radius = sph.getRadius();

   // Calculate the min and max points for the box
   math::Point<DATA_TYPE, 3> min_pt(center[0] - radius,
                                    center[1] - radius,
                                    center[2] - radius);
   math::Point<DATA_TYPE, 3> max_pt(center[0] + radius,
                                    center[1] + radius,
                                    center[2] + radius);

   box.setMin(min_pt);
   box.setMax(max_pt);
   box.setEmpty(radius == DATA_TYPE(0));
}


}

#endif
