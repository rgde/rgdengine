#pragma once

#include <rgde/render/particles/emitter.h>
#include <rgde/math/random.h>
#include <rgde/math/interpolyator.h>


namespace particles
{

class processor;
struct particle;


class  base_emitter : public emitter
{
public:
	typedef std::list<processor*> processors_list;
	typedef processors_list::iterator processors_iter;

	base_emitter(emitter::Type);
	virtual ~base_emitter();

	virtual void		getParticle(particle& p);

	void				reset();
	void				update(float dt);
	void				render();
	virtual void		debugDraw() = 0;

	void				addProcessor(processor*	 proc);	
	void				deleteProcessor(processor* proc);	
	
protected:
	virtual void toStream(io::IWriteStream& wf) const;
	virtual void fromStream(io::IReadStream& rf);

// Акксессоры
public:
	inline processors_list& getProcessors() { return m_lProcessors; }

	inline float getTime() { return m_fTimeNormalaized; }
	inline math::Vec3f&	getSpeed() {return m_vCurSpeed;}

	// Акксессоры задания
	void setFade(bool b);

	// Акксессоры к интерполяторам
	inline math::FloatInterp& particleMass()			{ return m_PMass; }
	inline math::FloatInterp& particleMassSpread()		{ return m_PMassSpread; }
	inline math::FloatInterp& particleRotationSpread()	{ return m_PRotationSpread; }
	inline math::FloatInterp& particleVelocity()		{ return m_PVelocity; }
	inline math::FloatInterp& particleVelocitySpread()	{ return m_PVelSpread; }
	inline math::Vec3Interp& particleAcceleration()		{ return m_PAcceleration; }
	inline math::Vec3Interp& getGlobalVelocityInterp()	{ return m_GlobalVelocity; }

	// Акксессоры получения / задания свойств
	inline float getCycleTime() const { return m_fCycleTime; }
	inline void setCycleTime(float fTime) { m_fCycleTime = fTime; }

	inline bool isCycling() const { return m_bIsCycling; }
	inline void setCycling(bool b) { m_bIsCycling = b; }

	inline bool	isVisible() const {return m_bIsVisible;}
	inline void	hide() { m_bIsVisible = false; }
	inline void	show() { m_bIsVisible = true; }

	inline float getTimeShift() const { return m_time_shift; }
	inline void setTimeShift(float t) { m_time_shift = t; }


protected:
	math::UnitRandom2k	m_Rand;

	processors_list	m_lProcessors;				// присоединенные процессоры частиц

	float			m_fCycleTime;				// время повтора для всех интерполяторов
	bool			m_bIsCycling;
	bool			m_bIsVisible;
	float			m_time_shift;				// смещение в секундах от начала проигрывания эффекта
	std::string		m_name;					// для будущего использования

	// common for all emmiters types modifiers
	math::FloatInterp	m_PMass;				// Масса частицы
	math::FloatInterp	m_PMassSpread;			// Произвольный разброс по массе частиц
	math::FloatInterp	m_PRotationSpread;		// разброс поворота
	math::FloatInterp	m_PVelocity;			// скорость вылета
	math::FloatInterp	m_PVelSpread;			// разброс скорости
	math::Vec3Interp	m_PAcceleration;		// ускорение частицы
	math::Vec3Interp	m_GlobalVelocity;		// глобальная скорость эмиттера
	
	// temporary computing values
	float			m_fTimeNormalaized;			// От 0 до 1 - текущее нормализованное время
	float			m_fCurrentTime;				// текущее время (меньше времени повтора)
	bool			m_bIsEnded;					// флаг: емиттер отработал

	math::Vec3f		m_vCurSpeed;
	math::Vec3f		m_vCurSpeedTransformed;
	math::Vec3f		m_vCurDisplacement;
	math::Vec3f		m_vOldPos;

	math::Vec3f		m_vAccelerationPrecomputed;
	math::Vec3f		m_vPAcceleration;
	math::Vec3f		m_vGlobalVelPrecomputed;
	math::Vec3f		m_vGlobalVel;
};

}