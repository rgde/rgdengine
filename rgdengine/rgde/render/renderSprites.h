#pragma once

//#include <rgde/base/singelton.h>

#include <rgde/math/mathTypes3d.h>

#include <rgde/render/renderDevice.h>
#include <rgde/render/renderManager.h>
#include <rgde/render/renderGeometry.h>

namespace render
{
	typedef boost::shared_ptr<class ITexture> PTexture;
	typedef boost::shared_ptr<class IEffect> PEffect;

	struct SSprite
	{
		math::Rect rect;					///> Прямоугольник текстурных координат спрайта
		math::Vec2f pos;					///> Позиция спрайта (в экранных координатах)
		math::Vec2f size;					///> Размер спрайта (в экранных координатах)
		float spin;							///< Поворот
		unsigned long uPriority;			///> Приоритет отрисовки
		render::PTexture pTexture;			///> Адрес текстуры
		math::Color color;					///> Цвет

		SSprite();
		SSprite( const math::Vec2f& pos_, const math::Vec2f& size_, 
			const math::Color& color_ = 0xffffffff,render::PTexture pTexture_ = render::PTexture(), 
			float spin_ = 0, const math::Rect& rect_ = math::Rect(0, 0, 1, 1),			
			unsigned long uPriority_ = 0 );
	};

	class CSpriteManager : public IDeviceObject, public IRendererable
	{
	public:
		typedef std::vector<SSprite> TSprites;
		typedef TSprites::iterator TSpritesIter;

		CSpriteManager(int priority = 0);
		~CSpriteManager();
	
		void setAditiveBlending(bool bAditive) { m_bAditive = bAditive; }

		inline TSprites& getSprites() { return m_vSprites; }
		inline unsigned getNumSpritesRendered() { return m_nSpritesRendered; }

		inline math::Vec2f& getOrigin() { return m_vOrigin; }
		inline void			setOrigin(math::Vec2f& vNewOrigin) { m_vOrigin = vNewOrigin; }

		virtual void addSprite(const SSprite& pSprite);

	protected:
		void render();
		void update();
		
		virtual void onLostDevice();
		virtual void onResetDevice();

	protected:
		bool m_bAditive;

		TSprites m_vSprites;						// Спрайты
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

	typedef base::TSingelton<CSpriteManager> TheSpriteManager;
}