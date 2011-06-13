#pragma once

#include <rgde/math/random.h>
#include <rgde/math/interpolyator.h>


namespace particles
{

class processor;
struct particle;

class  base_emitter : public math::frame
{
public:
	enum type_t {	spherical, box };

	typedef std::list<processor*> processors_list;
	typedef processors_list::iterator processors_iter;
	
	virtual ~base_emitter();

	virtual void		get_particle(particle& p);

	inline type_t type() const { return m_type; }

	void				reset();
	void				update(float dt);
	void				render();
	virtual void		debug_draw() = 0;

	void				add(processor*	 proc);	
	void				remove(processor* proc);

	inline processors_list& processors() { return m_processors; }

	inline float getTime() { return m_normalized_time; }
	inline math::vec3f&	getSpeed() {return m_vCurSpeed;}

	void fade(bool b);

	inline math::interpolatorf& particleMass()			{ return m_PMass; }
	inline math::interpolatorf& particleMassSpread()		{ return m_PMassSpread; }
	inline math::interpolatorf& particleRotationSpread()	{ return m_PRotationSpread; }
	inline math::interpolatorf& particle_velocity()		{ return m_velocity; }
	inline math::interpolatorf& particleVelocitySpread()	{ return m_PVelSpread; }
	inline math::interpolator_v3f& particleAcceleration()		{ return m_PAcceleration; }
	inline math::interpolator_v3f& getGlobalVelocityInterp()	{ return m_GlobalVelocity; }

	inline float getCycleTime() const { return m_fCycleTime; }
	inline void setCycleTime(float fTime) { m_fCycleTime = fTime; }

	inline bool looped() const { return m_looped; }
	inline void looped(bool b) { m_looped = b; }

	inline bool	visible() const {return m_visible;}
	inline void	visible(bool flag) { m_visible = flag; }
	inline void	hide() { visible(false); }
	inline void	show() { visible(true); }

	inline float start_delay() const { return m_start_delay; }
	inline void start_delay(float time) { m_start_delay = time; }

protected:
	explicit base_emitter(type_t);

	virtual void to_stream(io::write_stream& wf) const;
	virtual void from_stream(io::read_stream& rf);


protected:
	math::unit_rand_2k	m_Rand;

	processors_list	m_processors;				// присоединенные процессоры частиц

	float			m_fCycleTime;				// врем€ повтора дл€ всех интерпол€торов
	bool			m_looped;
	bool			m_visible;
	float			m_start_delay;				// смещение в секундах от начала проигрывани€ эффекта
	std::string		m_name;						// дл€ будущего использовани€

	// common for all emmiters types modifiers
	math::interpolatorf	m_PMass;				// ћасса частицы
	math::interpolatorf	m_PMassSpread;			// ѕроизвольный разброс по массе частиц
	math::interpolatorf	m_PRotationSpread;		// разброс поворота
	math::interpolatorf	m_velocity;			// скорость вылета
	math::interpolatorf	m_PVelSpread;			// разброс скорости
	math::interpolator_v3f	m_PAcceleration;		// ускорение частицы
	math::interpolator_v3f	m_GlobalVelocity;		// глобальна€ скорость эмиттера
	
	// temporary computing values
	float			m_normalized_time;			// ќт 0 до 1 - текущее нормализованное врем€
	float			m_time;				// текущее врем€ (меньше времени повтора)
	bool			m_bIsEnded;					// флаг: емиттер отработал

	math::vec3f		m_vCurSpeed;
	math::vec3f		m_vCurSpeedTransformed;
	math::vec3f		m_vCurDisplacement;
	math::vec3f		m_vOldPos;

	math::vec3f		m_vAccelerationPrecomputed;
	math::vec3f		m_vPAcceleration;
	math::vec3f		m_vGlobalVelPrecomputed;
	math::vec3f		m_vGlobalVel;

	const type_t		m_type;					// emitter type
};

}