/**--------------------------------------------------------------------------------------

									RGDE Engine

-----------------------------------------------------------------------------------------
\author Denis V. Ovod, 2003

\author Andrew "Zlobnik" Chouprina, 2005
mail:	dxprg@mail.ru


Created:			17:9:2003 (Denis V. Ovod)
Modified for RGDE:	march-april 2005 (Zlobnik)
---------------------------------------------------------------------------------------*/
#pragma once

#include <rgde/render/particles/abstract_emitter.h>


namespace particles{

	class  SphericalEmitter : public AbstractEmitter
	{
	public: 
		SphericalEmitter();
		virtual ~SphericalEmitter();

		void		 debugDraw();
		virtual void getParticle(Particle& p);
	
	protected:
		virtual void toStream(io::IWriteStream& wf) const;
		virtual void fromStream(io::IReadStream& rf);

	// Акксессоры
	public:
		inline math::FloatInterp& getRadiusInterp()			{ return m_Radius; }
		inline math::FloatInterp& getRadiusSpreadInterp()	{ return m_RadiusSpread; }
		inline math::FloatInterp& getAngleInterp()			{ return m_Angle; }


	protected:
		math::FloatInterp m_Radius;				// радиус
		math::FloatInterp m_RadiusSpread;		// разброс радиуса
		math::FloatInterp m_Angle;				// угол 
	};
}