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


	class  processor : public io::serialized_object, public core::meta_class
	{
		// need to manualy increment after each file format change (for code simplisity)
		static const unsigned file_version = 1004;
	public:
		inline void setEmitter(base_emitter* em) { m_pParentEmitter = em; }
		
		processor(base_emitter* em = 0);
		virtual ~processor();

		void load();

		void render();
		void update(float dt);
		virtual void debugDraw();
		void reset();


		const math::Matrix44f& getLTM();

		void setTextureName(const std::string& texName);

		// interpolators
		inline math::FloatInterp& particleRate()				{ return m_PRate; }
		inline math::FloatInterp& particleResistance()			{ return m_PResistance; }
		inline math::FloatInterp& particleSpin()				{ return m_PSpin; }
		inline math::FloatInterp& particleSpinSpread()			{ return m_PSpinSpread; }
		inline math::FloatInterp& particleLife()				{ return m_PLife; }
		inline math::FloatInterp& particleLifeSpread()			{ return m_PLifeSpread; }
		inline math::FloatInterp& particleSize()				{ return m_PSize; }
		inline math::ColorInterp& particleColorAlpha()			{ return m_PColorAlpha; }
		inline math::Vec3Interp& particleActingForce()			{ return m_PActingForce; }
		inline math::Vec3Interp& particleVelocity()				{ return m_PVelocity; }
		inline math::Vec3Interp& particleInitialVelSpread()		{ return m_PInitialVelSpread; }
		inline math::FloatInterp& particleVelSpreadAmplifier()	{ return m_PVelSpreadAmplifier; }

		// getters/setters
		inline bool isGlobal() const { return m_bIsGlobal; }
		inline void setGlobal(bool b) { m_bIsGlobal = b; }

		inline bool isFading() const { return m_bIsFading; }
		inline void setFade(bool b) { m_bIsFading = b; }

		inline unsigned getMaxParticles() const { return m_nMaxParticles; }
		inline void setMaxParticles (unsigned num) { 
			m_nMaxParticles = num; 
			m_Particles.resize(m_nMaxParticles);
		}

		inline int getSeed() const { return m_nMaxParticles; }
		inline void setSeed(int seed) { m_iRndSeed = seed; }

		inline bool isVisible() const { return m_bIsVisible; }
		inline void setVisible(bool visible) { m_bIsVisible = visible; }

		inline bool getIntenseMode() const { return m_bIntense; }
		inline void setIntenseMode(bool intense) { m_bIntense = intense; }

		inline bool getSparkMode() { return m_bIsSparks; }
		inline void setSparkMode(bool sm) { m_bIsSparks = sm; }

	protected:
		void loadTexture();
		void initPTank();
		inline void assignChilds();
		inline void createParticle(particle& p);

		virtual void geomRender();
		void fistTimeInit();
		void updateParticle(particle& p);
		void formTank();
		void addNewParticles(int num2add);

		virtual void toStream(io::write_stream& wf) const;
		virtual void fromStream(io::read_stream& rf);

	protected:
		renderer* m_spTank;
		render::texture_ptr m_texture;

		//////////////////////////////////////////////////////////////////////////
		// для работы с анимированной текстурой
		//////////////////////////////////////////////////////////////////////////
		// флаг выставляется автоматически если загруженная текстура 
		// определена как анимированная
		bool m_bIsAnimTextureUsed;
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
		bool m_bIsTexAnimCycled;
		// скорость воспроизведения текстурной анимации
		// в кадрах в секунду, время вычисляется по "возрасту" частицы
		unsigned m_cTexFps;
		// animation m_Texture frame random shift 
		unsigned m_nRndFrame;
		// is play animation, if not - will randomize m_Texture
		bool m_bIsPlayTexAnimation;
		//////////////////////////////////////////////////////////////////////////

		base_emitter* m_pParentEmitter;

		math::UnitRandom2k  rnd;

		int m_iRndSeed;

		math::FloatInterp m_PRate;						// Число излучаемых частиц в единицу времени
		math::FloatInterp m_PResistance;				// Трение (торможение)
		math::FloatInterp m_PSpin;						// Вращение
		math::FloatInterp m_PSpinSpread;
		math::FloatInterp m_PLife;						// Время жизни (Time to live, TTL)
		math::FloatInterp m_PLifeSpread;
		math::FloatInterp m_PSize;						// Размер 
		math::ColorInterp m_PColorAlpha;				// Цвет и альфа, 2 в 1
		math::Vec3Interp m_PActingForce;
		math::Vec3Interp m_PVelocity;					// Скорость (XYZ)
		math::Vec3Interp m_PInitialVelSpread;
		math::FloatInterp m_PVelSpreadAmplifier;		// Усилитель начального рандома скорости

		//std::string name; // для будущего использования
		std::string m_TexName;

		bool m_bIsFading;								// Затухает ли процессор (затухающий процессор не излучает новых частиц)
		bool m_bIsGlobal;								// Является ли процессор глобальным (т.е. не имеющим родительских трансформаций)

		bool m_bIntense;								// Меняет режим блендинга
		math::Vec3f m_fScaling;								// Масштабирование родительского эмитера
		int m_nMaxParticles;							// Максимальное число частиц в процессоре

		float m_ngkx;
		
		bool m_bIsSparks;
		bool m_bIsGeometric;
		//std::string m_DffName;
		//agl::PAtomic m_spGeom;

		particles_vector m_Particles;

		bool m_bModifiersLoaded;
		bool m_bIsVisible;
		bool m_bIsTexLoaded;
		bool m_bIsPtankInited;

		float m_fNormalizedTime;		// нормированное от 0 до 1
		float m_fRateAccum;				// собирает нецелые части от rate от кадра к кадру
		float m_dt;
	};

}