#include "stdafx.h"

#include "frustum.h"


namespace rgde
{
namespace math
{
	// We create an enum of the sides so we don't have to call each side 0 or 1.
	// This way it makes it more understandable and readable when dealing with frustum sides.
	enum frustum_plane
	{
		RIGHT	= 0,		// The RIGHT side of the frustum
		LEFT	= 1,		// The LEFT	 side of the frustum
		BOTTOM	= 2,		// The BOTTOM side of the frustum
		TOP		= 3,		// The TOP side of the frustum
		BACK	= 4,		// The BACK	side of the frustum
		FRONT	= 5			// The FRONT side of the frustum
	};  

	// Like above, instead of saying a number for the ABC and D of the plane, we
	// want to be more descriptive.
	enum plane_data
	{
		A = 0,				// The X value of the plane's normal
		B = 1,				// The Y value of the plane's normal
		C = 2,				// The Z value of the plane's normal
		D = 3				// The distance the plane is from the origin
	};
	
	/////////////////////////////////// NORMALIZE PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	///////
	///////	This normalizes a plane (A side) from a given frustum.
	///////
	/////////////////////////////////// NORMALIZE PLANE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	void normalize_plane(float frustum[6][4], int side)
	{
		// Here we calculate the magnitude of the normal to the plane (point A B C)
		// Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
		// To calculate magnitude you use the equation:  magnitude = sqrt( x^2 + y^2 + z^2)
		float magnitude = (float)sqrt( frustum[side][A] * frustum[side][A] + 
									   frustum[side][B] * frustum[side][B] + 
									   frustum[side][C] * frustum[side][C] );

		// Then we divide the plane's values by it's magnitude.
		// This makes it easier to work with.
		frustum[side][A] /= magnitude;
		frustum[side][B] /= magnitude;
		frustum[side][C] /= magnitude;
		frustum[side][D] /= magnitude; 
	}

	frustum::frustum()
	{
	}

	/////////////////////////////////// CALCULATE FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	///////
	///////	This extracts our frustum from the projection and modelview matrix.
	///////
	/////////////////////////////////// CALCULATE FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	void frustum::calculate(const mat44f& view, const mat44f& proj)
	{    
		mat44f mclip = proj*view;
		// Now we actually want to get the sides of the frustum.  To do this we take
		// the clipping planes we received above and extract the sides from them.

		//mclip = transpose(mclip);

		const float* clip = mclip.getData();

		// This will extract the RIGHT side of the frustum
		m_frustum[RIGHT][A] = clip[ 3] - clip[ 0];
		m_frustum[RIGHT][B] = clip[ 7] - clip[ 4];
		m_frustum[RIGHT][C] = clip[11] - clip[ 8];
		m_frustum[RIGHT][D] = clip[15] - clip[12];

		// Now that we have a normal (A,B,C) and a distance (D) to the plane,
		// we want to normalize that normal and distance.
		// Normalize the RIGHT side
		normalize_plane(m_frustum, RIGHT);

		// This will extract the LEFT side of the frustum
		m_frustum[LEFT][A] = clip[ 3] + clip[ 0];
		m_frustum[LEFT][B] = clip[ 7] + clip[ 4];
		m_frustum[LEFT][C] = clip[11] + clip[ 8];
		m_frustum[LEFT][D] = clip[15] + clip[12]; 		
		
		// Normalize the LEFT side
		normalize_plane(m_frustum, LEFT);

		// This will extract the BOTTOM side of the frustum
		m_frustum[BOTTOM][A] = clip[ 3] + clip[ 1];
		m_frustum[BOTTOM][B] = clip[ 7] + clip[ 5];
		m_frustum[BOTTOM][C] = clip[11] + clip[ 9];
		m_frustum[BOTTOM][D] = clip[15] + clip[13]; 

		// Normalize the BOTTOM side
		normalize_plane(m_frustum, BOTTOM); 	
		
		// This will extract the TOP side of the frustum
		m_frustum[TOP][A] = clip[ 3] - clip[ 1];
		m_frustum[TOP][B] = clip[ 7] - clip[ 5];
		m_frustum[TOP][C] = clip[11] - clip[ 9];
		m_frustum[TOP][D] = clip[15] - clip[13];

		// Normalize the TOP side
		normalize_plane(m_frustum, TOP);
		
		// This will extract the BACK side of the frustum
		m_frustum[BACK][A] = clip[ 3] - clip[ 2];
		m_frustum[BACK][B] = clip[ 7] - clip[ 6];
		m_frustum[BACK][C] = clip[11] - clip[10];
		m_frustum[BACK][D] = clip[15] - clip[14];

		// Normalize the BACK side
		normalize_plane(m_frustum, BACK);

		// This will extract the FRONT side of the frustum
		m_frustum[FRONT][A] = clip[ 3] + clip[ 2];
		m_frustum[FRONT][B] = clip[ 7] + clip[ 6];
		m_frustum[FRONT][C] = clip[11] + clip[10];
		m_frustum[FRONT][D] = clip[15] + clip[14]; 

		// Normalize the FRONT side
		normalize_plane(m_frustum, FRONT);
	} 	
	
	// The code below will allow us to make checks within the frustum.  For example,
	// if we want to see if a point, a sphere, or a cube lies inside of the frustum.
	// Because all of our planes point INWARDS (The normals are all pointing inside the frustum)
	// we then can assume that if a point is in FRONT of all of the planes, it's inside.

	/////////////////////////////////// POINT IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	///////
	///////	This determines if a point is inside of the frustum
	///////
	/////////////////////////////////// POINT IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	bool frustum::test_point( float x, float y, float z ) const
	{
		// Go through all the sides of the frustum
		for(int i = 0; i < 6; i++ )
		{
			// Calculate the plane equation and check if the point is behind a side of the frustum
			if(m_frustum[i][A] * x + m_frustum[i][B] * y + m_frustum[i][C] * z + m_frustum[i][D] <= 0)
			{
				// The point was behind a side, so it ISN'T in the frustum
				return false;
			}
		} 		// The point was inside of the frustum (In front of ALL the sides of the frustum)
		return true;
	} 
	/////////////////////////////////// SPHERE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	///////
	///////	This determines if a sphere is inside of our frustum by it's center and radius.
	///////
	/////////////////////////////////// SPHERE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	bool frustum::test_sphere( float x, float y, float z, float radius ) const
	{
		// Go through all the sides of the frustum
		for(int i = 0; i < 6; i++ )	
		{
			// If the center of the sphere is farther away from the plane than the radius
			if( m_frustum[i][A] * x + m_frustum[i][B] * y + m_frustum[i][C] * z + m_frustum[i][D] <= -radius )
			{
				// The distance was greater than the radius so the sphere is outside of the frustum
				return false;
			}
		} 		// The sphere was inside of the frustum!
		return true;
	} 
	/////////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	///////
	///////	This determines if a cube is in or around our frustum by it's center and 1/2 it's length
	///////
	/////////////////////////////////// CUBE IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
	bool frustum::test_box( float x, float y, float z, float size ) const
	{
		// Basically, what is going on is, that we are given the center of the cube,
		// and half the length.  Think of it like a radius.  Then we checking each point
		// in the cube and seeing if it is inside the frustum.  If a point is found in front
		// of a side, then we skip to the next side.  If we get to a plane that does NOT have
		// a point in front of it, then it will return false. 		// *Note* - This will sometimes say that a cube is inside the frustum when it isn't.
		// This happens when all the corners of the bounding box are not behind any one plane.
		// This is rare and shouldn't effect the overall rendering speed.

		for(int i = 0; i < 6; i++ )
		{
			if(m_frustum[i][A] * (x - size) + m_frustum[i][B] * (y - size) + m_frustum[i][C] * (z - size) + m_frustum[i][D] > 0)
			   continue;
			if(m_frustum[i][A] * (x + size) + m_frustum[i][B] * (y - size) + m_frustum[i][C] * (z - size) + m_frustum[i][D] > 0)
			   continue;
			if(m_frustum[i][A] * (x - size) + m_frustum[i][B] * (y + size) + m_frustum[i][C] * (z - size) + m_frustum[i][D] > 0)
			   continue;
			if(m_frustum[i][A] * (x + size) + m_frustum[i][B] * (y + size) + m_frustum[i][C] * (z - size) + m_frustum[i][D] > 0)
			   continue;
			if(m_frustum[i][A] * (x - size) + m_frustum[i][B] * (y - size) + m_frustum[i][C] * (z + size) + m_frustum[i][D] > 0)
			   continue;
			if(m_frustum[i][A] * (x + size) + m_frustum[i][B] * (y - size) + m_frustum[i][C] * (z + size) + m_frustum[i][D] > 0)
			   continue;
			if(m_frustum[i][A] * (x - size) + m_frustum[i][B] * (y + size) + m_frustum[i][C] * (z + size) + m_frustum[i][D] > 0)
			   continue;
			if(m_frustum[i][A] * (x + size) + m_frustum[i][B] * (y + size) + m_frustum[i][C] * (z + size) + m_frustum[i][D] > 0)
			   continue;

			// If we get here, it isn't in the frustum
			return false;
		} 		return true;
	}
}
}