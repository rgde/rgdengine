#include "stdafx.h"
#include <math.h>

#include "ArcBall.h"

//Arcball sphere constants:
//Diameter is       2.0f
//Radius is         1.0f
//Radius squared is 1.0f

namespace rgde
{
	void ArcBall::map2sphere(int x, int y, math::vec3f& new_vec) const
	{
		//Copy paramter into temp point
		math::vec2f TempPt((float)x,(float)y);

		//Adjust point coords and scale down to range of [-1 ... 1]
		TempPt[0]  =        (TempPt[0] * AdjustWidth)  - 1.0f;
		TempPt[1]  = 1.0f - (TempPt[1] * AdjustHeight);

		TempPt[0] *= -0.1f;
		TempPt[1] *= -0.1f;


		//Compute the square of the length of the vector to the point from the center
		float length    = (TempPt[0] * TempPt[0]) + (TempPt[1] * TempPt[1]);

		//If the point is mapped outside of the sphere... (length > radius squared)
		if (length > 1.0f)
		{
			float norm;

			//Compute a normalizing factor (radius / sqrt(length))
			norm    = 1.0f / sqrtf(length);

			//Return the "normalized" vector, a point on the sphere
			new_vec[0] = TempPt[0] * norm;
			new_vec[1] = TempPt[1] * norm;
			new_vec[2] = 0.0f;
		}
		else    //Else it's on the inside
		{
			//Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
			new_vec[0] = TempPt[0];
			new_vec[1] = TempPt[1];
			new_vec[2] = sqrtf(1.0f - length);
		}
	}

	ArcBall::ArcBall(float NewWidth, float NewHeight)
		: m_accum_quaternion(0,0,0,1)
	{
		//Clear initial values
		StVec[0]     =
		StVec[1]     = 
		StVec[2]     = 

		EnVec[0]     =
		EnVec[1]     = 
		EnVec[2]     = 0.0f;

		//Set initial bounds
		set_bounds(NewWidth, NewHeight);
		m_matrix = math::MAT_IDENTITY44F;
		//D3DXMatrixIsIdentity(&m_matrix);
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
		//D3DXVECTOR3 Perp;
		math::vec3f Perp;

		//Compute the vector perpendicular to the begin and end vectors	
		Perp = math::makeCross(StVec, EnVec);
		//D3DXVec3Cross(&Perp, &StVec, &EnVec);


		//Compute the length of the perpendicular vector
		//if (D3DXVec3Length(&Perp) > 0.0001f)    //if its non-zero
		if (math::length(Perp) > 0.0001f)    //if its non-zero
		{
			//We're ok, so return the perpendicular vector as the transform after all
			m_quaternion[0] = Perp[0];
			m_quaternion[1] = Perp[1];
			m_quaternion[2] = Perp[2];
			//In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
			//m_quaternion.w= D3DXVec3Dot(&StVec, &EnVec);
			m_quaternion[3] = math::dot(StVec, EnVec);
		}
		else                                    //if its zero
		{
			//The begin and end vectors coincide, so return an identity transform
			m_quaternion = math::QUAT_ADD_IDENTITYF; // (0,0,0,0);
			//D3DXQuaternionIdentity(&m_quaternion);
		}

		math::normalize(m_accum_quaternion);
		//D3DXQuaternionNormalize(&m_accum_quaternion, &m_accum_quaternion);
		math::normalize(m_quaternion);
		//D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

		m_accum_quaternion *= m_quaternion;

		//D3DXMATRIX temp;
		//m_matrix = math::makeRot(m_accum_quaternion);
		math::set(m_matrix, m_accum_quaternion);
		//D3DXMatrixRotationQuaternion( &m_matrix, &m_accum_quaternion);	
		//m_matrix = m_matrix * temp;
	}
}