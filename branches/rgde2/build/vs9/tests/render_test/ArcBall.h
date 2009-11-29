#pragma once

#include <rgde/core/math.h>

namespace rgde
{
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

		const math::quatf& get_quaternion() const {return m_accum_quaternion;}
		const math::mat44f& get_matrix() const {return m_matrix;}

	protected:
		void map2sphere(int x, int y, math::vec3f& new_vec) const;

	protected:
		math::quatf m_quaternion;
		math::quatf m_accum_quaternion;

		math::mat44f m_matrix;

		math::vec3f StVec;          //Saved click vector
		math::vec3f EnVec;          //Saved drag vector

		float     AdjustWidth;    //Mouse bounds width
		float     AdjustHeight;   //Mouse bounds height
	};
}