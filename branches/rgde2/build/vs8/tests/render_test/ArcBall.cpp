#include "stdafx.h"
#include <math.h>

#include "ArcBall.h"

//Arcball sphere constants:
//Diameter is       2.0f
//Radius is         1.0f
//Radius squared is 1.0f

void ArcBall::map2sphere(int x, int y, D3DXVECTOR3& new_vec) const
{
	//Copy paramter into temp point
	D3DXVECTOR2 TempPt((float)x,(float)y);

	//Adjust point coords and scale down to range of [-1 ... 1]
	TempPt.x  =        (TempPt.x * AdjustWidth)  - 1.0f;
	TempPt.y  = 1.0f - (TempPt.y * AdjustHeight);

	TempPt.x *= -0.1f;
	TempPt.y *= -0.1f;


	//Compute the square of the length of the vector to the point from the center
	float length    = (TempPt.x * TempPt.x) + (TempPt.y * TempPt.y);

	//If the point is mapped outside of the sphere... (length > radius squared)
	if (length > 1.0f)
	{
		float norm;

		//Compute a normalizing factor (radius / sqrt(length))
		norm    = 1.0f / sqrtf(length);

		//Return the "normalized" vector, a point on the sphere
		new_vec.x = TempPt.x * norm;
		new_vec.y = TempPt.y * norm;
		new_vec.z = 0.0f;
	}
	else    //Else it's on the inside
	{
		//Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
		new_vec.x = TempPt.x;
		new_vec.y = TempPt.y;
		new_vec.z = sqrtf(1.0f - length);
	}
}

ArcBall::ArcBall(float NewWidth, float NewHeight)
: m_accum_quaternion(0,0,0,1)
{
	//Clear initial values
	StVec.x     =
	StVec.y     = 
	StVec.z     = 

	EnVec.x     =
	EnVec.y     = 
	EnVec.z     = 0.0f;

	//Set initial bounds
	set_bounds(NewWidth, NewHeight);
	D3DXMatrixIsIdentity(&m_matrix);
}

void ArcBall::click(int x, int y)
{
	map2sphere(x, y, StVec);
}

void ArcBall::drag(int x, int y)
{
	//Map the point to the sphere
	map2sphere(x, y, EnVec);

	//Return the quaternion equivalent to the rotation
	D3DXVECTOR3 Perp;

	//Compute the vector perpendicular to the begin and end vectors	
	D3DXVec3Cross(&Perp, &StVec, &EnVec);

	//Compute the length of the perpendicular vector
	if (D3DXVec3Length(&Perp) > 0.0001f)    //if its non-zero
	{
		//We're ok, so return the perpendicular vector as the transform after all
		m_quaternion.x = Perp.x;
		m_quaternion.y = Perp.y;
		m_quaternion.z = Perp.z;
		//In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
		m_quaternion.w= D3DXVec3Dot(&StVec, &EnVec);
	}
	else                                    //if its zero
	{
		//The begin and end vectors coincide, so return an identity transform
		D3DXQuaternionIdentity(&m_quaternion);
		//m_quaternion.x = 
		//	m_quaternion.y = 
		//	m_quaternion.z = 
		//	m_quaternion.w = 0.0f;
	}

	D3DXQuaternionNormalize(&m_accum_quaternion, &m_accum_quaternion);
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	m_accum_quaternion *= m_quaternion;

	//D3DXMATRIX temp;
	D3DXMatrixRotationQuaternion( &m_matrix, &m_accum_quaternion);	
	//m_matrix = m_matrix * temp;
}