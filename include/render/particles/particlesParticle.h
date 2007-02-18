#pragma once

#include "math/mathTypes3d.h"

namespace particles{

	// самая обычная частица с типичными параметрами, для использования по 
	// умолчанию любые "навороченные" частицы должны быть иметь базовым 
	// классом этот класс частицы	
	struct BaseParticle
	{
		math::Vec3f pos;
		float size;						// size randomness
		math::Color color;			// color randomness
		float rotation;					// rotation randomness
		
		BaseParticle() 
			: size(0), rotation(0), color(math::Color(255, 255, 255, 255))
		{
		}
	};

	// класс "продвинутой" частицы
	struct Particle : public BaseParticle
	{
		float mass;
		float ttl;				// ttl randomness
		float time;
		float m_fOldTime;
		bool  dead;
		float rot_speed;
		float initial_spin;
		float cur_tex_frame;

		math::Vec3f vel;
		math::Vec3f vel_spread;
		math::Vec3f initial_vel;
		math::Vec3f initial_pos;
		math::Vec3f sum_vel;
		math::Vec3f old_pos;		

		Particle() : dead(true), initial_spin(0), rot_speed(0), 
					 mass(0), time(0), ttl(0), cur_tex_frame(0)
		{
		}
	};

	typedef std::vector<Particle> TParticles;
	typedef TParticles::iterator  TParticlesIter;

} //~ namespace pfx