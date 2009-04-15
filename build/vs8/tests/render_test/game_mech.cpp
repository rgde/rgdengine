#include "stdafx.h"
#include "game_mech.h"
#include "Application.h"

namespace game
{
	grade::grade() 
		: time(0)
		, abs(false)
		, score(-1)
		, mult(0)
		, color(rgde::math::color::White)
	{
	}

	const grade& game_mech::get_grade(float dt) const
	{
		for(int i = m_grades.size()-1; i >= 0; --i)
		{
			const grade& g = m_grades[i];
			if (g.time > (g.abs ? abs(dt) : dt) && (dt>=0 || g.abs))
			{
				return g;
			}
		}

		static grade empty_grade;
		return empty_grade;
	}

	void game_mech::read(xml::node n)
	{
		xml::node grade_node = n("grade");
		for (;grade_node; grade_node = grade_node.next_sibling("grade"))
		{
			grade g;
			read(grade_node, g);
			m_grades.push_back(g);
		}
	}

	void game_mech::read(xml::node node, grade& g)
	{
		g.time = node["time"].as_float()*10;
		g.abs = node["abs"].as_bool();
		g.mult = node["mult"].as_int();
		g.score = node["score"].as_int();
		g.color = rgde::xml_color(node("color"));
		g.bkcolor = rgde::xml_color(node("bkcolor"));
	}

	int game_mech::get_score(float dt)
	{
		const grade& g = get_grade(dt);
		return g.score;
	}

	rgde::math::color game_mech::get_color(float dt)
	{
		const grade& g = get_grade(dt);
		return g.color;
	}

	int game_mech::get_mult(float dt)
	{
		const grade& g = get_grade(dt);
		return g.mult;
	}
}