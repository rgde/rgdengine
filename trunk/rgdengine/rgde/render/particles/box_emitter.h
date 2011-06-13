#pragma once

#include <rgde/render/particles/abstract_emitter.h>

namespace particles
{
	class  box_emitter : public base_emitter
	{
	public:
		box_emitter();
		virtual ~box_emitter();

		virtual void get_particle(particle& p);
		void		 debug_draw();

	protected:
		virtual void to_stream(io::write_stream& wf) const;
		virtual void from_stream(io::read_stream& rf);

	// Акксессоры
	public:
		inline math::interpolator_v3f& getBoxSizeInterp()			{ return m_box_size; }
		inline math::interpolator_v3f& getBoxSizeSpreadInterp()	{ return m_box_size_spread; }
		inline math::interpolator_v3f& getDirectionInterp()		{ return m_direction; }
		inline math::interpolator_v3f& getDirectionSpreadInterp()	{ return m_direction_spread; }


	protected:
		math::interpolator_v3f m_box_size;
		math::interpolator_v3f m_box_size_spread;
		math::interpolator_v3f m_direction;
		math::interpolator_v3f m_direction_spread;
	};
}