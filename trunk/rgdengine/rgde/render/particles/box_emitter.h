#pragma once

#include <rgde/render/particles/abstract_emitter.h>



namespace particles
{
	class  box_emitter : public base_emitter
	{
	public:
		box_emitter();
		virtual ~box_emitter();

		virtual void getParticle(particle& p);
		void		 debugDraw();

	protected:
		virtual void toStream(io::IWriteStream& wf) const;
		virtual void fromStream(io::IReadStream& rf);

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