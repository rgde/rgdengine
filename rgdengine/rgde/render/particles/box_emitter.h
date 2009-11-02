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
		virtual void toStream(io::write_stream& wf) const;
		virtual void fromStream(io::read_stream& rf);

	// Акксессоры
	public:
		inline math::Vec3Interp& getBoxSizeInterp()			{ return m_BoxSize; }
		inline math::Vec3Interp& getBoxSizeSpreadInterp()	{ return m_BoxSizeSpread; }
		inline math::Vec3Interp& getDirectionInterp()		{ return m_Direction; }
		inline math::Vec3Interp& getDirectionSpreadInterp()	{ return m_DirectionSpread; }


	protected:
		math::Vec3Interp m_BoxSize;
		math::Vec3Interp m_BoxSizeSpread;
		math::Vec3Interp m_Direction;
		math::Vec3Interp m_DirectionSpread;
	};
}