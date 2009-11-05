#pragma once

//#include <rgde/base/singelton.h>

#include <rgde/math/types3d.h>

#include <rgde/render/render_device.h>
#include <rgde/render/manager.h>
#include <rgde/render/geometry.h>

namespace render
{
	typedef boost::shared_ptr<class texture> texture_ptr;
	typedef boost::shared_ptr<class effect> effect_ptr;

	struct sprite
	{
		math::Rect rect;					///> Прямоугольник текстурных координат спрайта
		math::vec2f pos;					///> Позиция спрайта (в экранных координатах)
		math::vec2f size;					///> Размер спрайта (в экранных координатах)
		float spin;							///< Поворот
		unsigned long priority;			///> Приоритет отрисовки
		render::texture_ptr texture;			///> Адрес текстуры
		math::Color color;					///> Цвет

		sprite();
		sprite( const math::vec2f& pos_, const math::vec2f& size_, 
			const math::Color& color_ = 0xffffffff,render::texture_ptr pTexture_ = render::texture_ptr(), 
			float spin_ = 0, const math::Rect& rect_ = math::Rect(0, 0, 1, 1),			
			unsigned long uPriority_ = 0 );
	};

	class sprite_manager : public device_object, public rendererable
	{
	public:
		typedef std::vector<sprite> sprites_vector;
		typedef sprites_vector::iterator SpritesIter;

		sprite_manager(int priority = 0);
		~sprite_manager();
	
		void setAditiveBlending(bool bAditive) { m_bAditive = bAditive; }

		inline sprites_vector& getSprites() { return m_sprites; }
		inline unsigned getNumSpritesRendered() { return m_nSpritesRendered; }

		inline math::vec2f& getOrigin() { return m_vOrigin; }
		inline void			setOrigin(math::vec2f& vNewOrigin) { m_vOrigin = vNewOrigin; }

		virtual void add_sprite(const sprite& pSprite);

	protected:
		void render();
		void update();
		
		virtual void onLostDevice();
		virtual void onResetDevice();

	protected:
		bool m_bAditive;

		sprites_vector m_sprites;						// Спрайты
		unsigned m_nSpritesRendered;				/// Число отрисованных в последний раз спрайтов

		effect_ptr  m_effect;

		typedef indexed_geometry<vertex::PositionTransformedColoredTextured, false> geometry;
		geometry m_geometry;						/// Геометрия

		unsigned m_nReservedSize;					/// Число спрайтов, под которое зарезервированы буферы

		bool m_bSorted;								/// Отсортированы ли спрайты в массиве по приоритету
		bool m_bUpdated;							/// Были ли спрайты добавлены / удалены
		
		const math::vec2f m_cvScreenSize;			// Разрешение экрана, для которого менеджер проводит внутренние вычисления
		math::vec2f m_vScale;						// Коэффиценты масштабирования разрешений взаимодействия с внешним миром на реальные экранные координаты
		math::vec2f m_vOrigin;

		/// Эти переменные используются только для хранения внутренних расчётов
		std::vector<unsigned> m_vEqualPrioritiesN;	/// Число спрайтов в группах с одинаковыми приоритетами
	};

	typedef base::singelton<sprite_manager> TheSpriteManager;
}