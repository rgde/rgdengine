#pragma once

namespace lines_test
{
	class point
	{
	public:
		point();

		void reset();
		void update();

		inline math::vec3f& get_pos() { return m_pos; }

	protected:
		math::vec3f m_pos;							///> Позиция
		math::vec3f m_vel;							///> Скорость
	};
}