#pragma once

namespace rgde
{

namespace math
{
	class frustum 
	{
	public:
		frustum();

		/// Call this every time the camera moves to update the frustum
		void calculate(const mat44f& view, const mat44f& proj);

		/// This takes a 3D point and returns TRUE if it's inside of the frustum
		bool test_point(float x, float y, float z) const;

		/// This takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum
		bool test_sphere(float x, float y, float z, float radius) const;

		/// This takes the center and half the length of the cube.
		bool test_box( float x, float y, float z, float size ) const;

	private:
		/// This holds the A B C and D values for each side of our frustum.
		float m_frustum[6][4];
	};

}

}