#pragma once

namespace lines_test
{
	class point
	{
	public:
		point();

		void reset();
		void update();

		inline math::Vec3f& get_pos() { return m_pos; }

	protected:
		math::Vec3f m_pos;							///> Позиция
		math::Vec3f m_vel;							///> Скорость
	};
}