/**--------------------------------------------------------------------------------------

									RGDE Engine

	-----------------------------------------------------------------------------
\author Denis V. Ovod, 2003

\author Andrew "Zlobnik" Chouprina, 2005
mail:	dxprg@mail.ru


Created:			17:9:2003 (Denis V. Ovod)
Modified for RGDE:	march-april 2005 (Zlobnik)
---------------------------------------------------------------------------------------*/
#pragma once

#include <rgde/render/particles/particle.h>
#include <rgde/math/random.h>


namespace particles{

	class base_emitter;
	class renderer;


	class  processor : public io::serialized_object
					 , public core::meta_class
	{
		// need to manually increment after each file format change (for code simplisity)
		static const unsigned file_version = 1004;
	public:		
		processor(base_emitter* em = 0);
		virtual ~processor();

		void load();

		void render();
		void update(float dt);
		virtual void debug_draw();
		void reset();

		inline void set_emitter(base_emitter* em) { m_parent_emitter = em; }

		const math::matrix44f& get_local_tm();

		void set_texture_name(const std::string& texture_name);

		// interpolators
		inline math::FloatInterp& particleRate()				{ return m_rate; }
		inline math::FloatInterp& particleResistance()			{ return m_PResistance; }
		inline math::FloatInterp& particleSpin()				{ return m_PSpin; }
		inline math::FloatInterp& particleSpinSpread()			{ return m_PSpinSpread; }
		inline math::FloatInterp& particleLife()				{ return m_life; }
		inline math::FloatInterp& particleLifeSpread()			{ return m_PLifeSpread; }
		inline math::FloatInterp& particleSize()				{ return m_size; }
		inline math::ColorInterp& particleColorAlpha()			{ return m_color_alpha; }
		inline math::Vec3Interp& particleActingForce()			{ return m_PActingForce; }
		inline math::Vec3Interp& particleVelocity()				{ return m_velocity; }
		inline math::Vec3Interp& particleInitialVelSpread()		{ return m_PInitialVelSpread; }
		inline math::FloatInterp& particleVelSpreadAmplifier()	{ return m_vel_spread_amp; }

		// getters/setters
		inline bool isGlobal() const { return m_is_global; }
		inline void setGlobal(bool b) { m_is_global = b; }

		inline bool is_fading() const { return m_is_fading; }
		inline void set_fade(bool b) { m_is_fading = b; }

		inline unsigned getMaxParticles() const { return m_nMaxParticles; }
		inline void setMaxParticles (unsigned num) { 
			m_nMaxParticles = num; 
			m_Particles.resize(m_nMaxParticles);
		}

		inline int getSeed() const { return m_nMaxParticles; }
		inline void set_seed(int seed) { m_rnd_seed = seed; }

		inline bool isVisible() const { return m_is_visible; }
		inline void setVisible(bool visible) { m_is_visible = visible; }

		inline bool getIntenseMode() const { return m_bIntense; }
		inline void setIntenseMode(bool intense) { m_bIntense = intense; }

		inline bool getSparkMode() { return m_is_sparks; }
		inline void setSparkMode(bool sm) { m_is_sparks = sm; }

	protected:
		void loadTexture();
		void initPTank();
		inline void assignChilds();
		inline void createParticle(particle& p);

		virtual void geomRender();
		void first_time_init();
		void updateParticle(particle& p);
		void formTank();
		void addNewParticles(int num2add);

		virtual void to_stream(io::write_stream& wf) const;
		virtual void from_stream(io::read_stream& rf);

	protected:
		renderer* m_tank;
		render::texture_ptr m_texture;

		//////////////////////////////////////////////////////////////////////////
		// для работы с анимированной текстурой
		//////////////////////////////////////////////////////////////////////////
		// флаг выставляется автоматически если загруженная текстура 
		// определена как анимированная
		bool m_is_anim_texture_used;
		// кол-во заполненных колонок
		unsigned char m_ucCol;
		// кол-во заполненных строк
		unsigned char m_ucRow;
		// кол-во колонок
		unsigned char m_ucTcol;
		// кол-во строк
		unsigned char m_ucTrow;
		// кол-во кадров 
		unsigned char m_ucTexFrames;
		// включено ли зацикливание текстурной анимации
		// если нет - то анимация растянута на время жизни партикла
		bool m_is_anim_texture_cycled;
		// скорость воспроизведения текстурной анимации
		// в кадрах в секунду, время вычисляется по "возрасту" частицы
		unsigned m_texture_fps;
		// animation m_Texture frame random shift 
		unsigned m_rnd_frame;
		// is play animation, if not - will randomize m_Texture
		bool m_is_play_tex_anim;
		//////////////////////////////////////////////////////////////////////////

		base_emitter* m_parent_emitter;

		math::unit_rand_2k  rnd;

		int m_rnd_seed;

		math::FloatInterp m_rate;						// Число излучаемых частиц в единицу времени
		math::FloatInterp m_PResistance;				// Трение (торможение)
		math::FloatInterp m_PSpin;						// Вращение
		math::FloatInterp m_PSpinSpread;
		math::FloatInterp m_life;						// Время жизни (Time to live, TTL)
		math::FloatInterp m_PLifeSpread;
		math::FloatInterp m_size;						// Размер 
		math::ColorInterp m_color_alpha;				// Цвет и альфа, 2 в 1
		math::Vec3Interp m_PActingForce;
		math::Vec3Interp m_velocity;					// Скорость (XYZ)
		math::Vec3Interp m_PInitialVelSpread;
		math::FloatInterp m_vel_spread_amp;		// Усилитель начального рандома скорости

		//std::string name; // для будущего использования
		std::string m_texture_name;

		bool m_is_fading;								// Затухает ли процессор (затухающий процессор не излучает новых частиц)
		bool m_is_global;								// Является ли процессор глобальным (т.е. не имеющим родительских трансформаций)

		bool m_bIntense;								// Меняет режим блендинга
		math::vec3f m_fScaling;								// Масштабирование родительского эмитера
		int m_nMaxParticles;							// Максимальное число частиц в процессоре

		float m_ngkx;
		
		bool m_is_sparks;
		bool m_is_geometric;
		//std::string m_DffName;
		//agl::PAtomic m_spGeom;

		particles_vector m_Particles;

		bool m_bModifiersLoaded;
		bool m_is_visible;
		bool m_is_texture_loaded;
		bool m_bIsPtankInited;

		float m_fNormalizedTime;		// нормированное от 0 до 1
		float m_rate_accum;				// собирает нецелые части от rate от кадра к кадру
		float m_dt;
	};

}