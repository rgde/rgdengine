#pragma once

namespace game
{
	struct grade
	{
		grade();
		float time;
		bool abs;
		int score;
		int mult;
		rgde::math::color color;
		rgde::math::color bkcolor;
	};

	class game_mech
	{
	public:
		game_mech(){}

		void read(xml::node n);

		int get_score(float dt);
		rgde::math::color get_color(float dt);
		int get_mult(float dt);

		const grade& get_grade(float dt) const;

		typedef std::vector<grade> grades;

	private:
		void read(xml::node node, grade& g);

	private:
		grades m_grades;
	};
}