#pragma once

//#include <rgde/base/singelton.h>

#include <rgde/math/types3d.h>

#include <rgde/render/device.h>
#include <rgde/render/manager.h>
#include <rgde/render/geometry.h>

namespace render
{
	typedef boost::shared_ptr<class ITexture> PTexture;
	typedef boost::shared_ptr<class Effect> PEffect;

	struct Sprite
	{
		math::Rect rect;					///> Прямоугольник текстурных координат спрайта
		math::Vec2f pos;					///> Позиция спрайта (в экранных координатах)
		math::Vec2f size;					///> Размер спрайта (в экранных координатах)
		float spin;							///< Поворот
		unsigned long uPriority;			///> Приоритет отрисовки
		render::PTexture texture;			///> Адрес текстуры
		math::Color color;					///> Цвет

		Sprite();
		Sprite( const math::Vec2f& pos_, const math::Vec2f& size_, 
			const math::Color& color_ = 0xffffffff,render::PTexture pTexture_ = render::PTexture(), 
			float spin_ = 0, const math::Rect& rect_ = math::Rect(0, 0, 1, 1),			
			unsigned long uPriority_ = 0 );
	};

	class SpriteManager : public IDeviceObject, public IRendererable
	{
	public:
		typedef std::vector<Sprite> SpritesVector;
		typedef SpritesVector::iterator SpritesIter;

		SpriteManager(int priority = 0);
		~SpriteManager();
	
		void setAditiveBlending(bool bAditive) { m_bAditive = bAditive; }

		inline SpritesVector& getSprites() { return m_sprites; }
		inline unsigned getNumSpritesRendered() { return m_nSpritesRendered; }

		inline math::Vec2f& getOrigin() { return m_vOrigin; }
		inline void			setOrigin(math::Vec2f& vNewOrigin) { m_vOrigin = vNewOrigin; }

		virtual void addSprite(const Sprite& pSprite);

	protected:
		void render();
		void update();
		
		virtual void onLostDevice();
		virtual void onResetDevice();

	protected:
		bool m_bAditive;

		SpritesVector m_sprites;						// Спрайты
		unsigned m_nSpritesRendered;				/// Число отрисованных в последний раз спрайтов

		PEffect  m_pEffect;

		typedef TIndexedGeometry<vertex::PositionTransformedColoredTextured, false> Geometry;
		Geometry m_Geometry;						/// Геометрия

		unsigned m_nReservedSize;					/// Число спрайтов, под которое зарезервированы буферы

		bool m_bSorted;								/// Отсортированы ли спрайты в массиве по приоритету
		bool m_bUpdated;							/// Были ли спрайты добавлены / удалены
		
		const math::Vec2f m_cvScreenSize;			// Разрешение экрана, для которого менеджер проводит внутренние вычисления
		math::Vec2f m_vScale;						// Коэффиценты масштабирования разрешений взаимодействия с внешним миром на реальные экранные координаты
		math::Vec2f m_vOrigin;

		/// Эти переменные используются только для хранения внутренних расчётов
		std::vector<unsigned> m_vEqualPrioritiesN;	/// Число спрайтов в группах с одинаковыми приоритетами
	};

	typedef base::TSingelton<SpriteManager> TheSpriteManager;
}