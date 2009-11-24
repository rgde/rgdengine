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
		inline math::interpolator_v3f& getBoxSizeSpreadInterp()	{ return m_BoxSizeSpread; }
		inline math::interpolator_v3f& getDirectionInterp()		{ return m_Direction; }
		inline math::interpolator_v3f& getDirectionSpreadInterp()	{ return m_DirectionSpread; }


	protected:
		math::interpolator_v3f m_box_size;
		math::interpolator_v3f m_BoxSizeSpread;
		math::interpolator_v3f m_Direction;
		math::interpolator_v3f m_DirectionSpread;
	};
}