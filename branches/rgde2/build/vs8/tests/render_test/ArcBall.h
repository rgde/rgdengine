#pragma once


#include <d3dx9math.h>

class ArcBall
{
public:
	ArcBall(float width, float height);
	~ArcBall() {}

	void set_bounds(float width, float height)
	{
		assert((width > 1.0f) && (height > 1.0f));

		//Set adjustment factor for width/height
		AdjustWidth  = 1.0f / ((width  - 1.0f) * 0.5f);
		AdjustHeight = 1.0f / ((height - 1.0f) * 0.5f);
	}

	void click(int x, int y);
	void drag(int x, int y);

	const D3DXQUATERNION& get_quaternion() const {return m_accum_quaternion;}
	const D3DXMATRIX& get_matrix() const {return m_matrix;}

protected:
	void map2sphere(int x, int y, D3DXVECTOR3& new_vec) const;

protected:
	D3DXQUATERNION m_quaternion;
	D3DXQUATERNION m_accum_quaternion;
	D3DXMATRIX m_matrix;

	D3DXVECTOR3   StVec;          //Saved click vector
	D3DXVECTOR3   EnVec;          //Saved drag vector

	float     AdjustWidth;    //Mouse bounds width
	float     AdjustHeight;   //Mouse bounds height
};
