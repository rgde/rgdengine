#pragma once

namespace rgde
{
	namespace render
	{
		struct sprite
		{
			math::rect rect;					///> Прямоугольник текстурных координат спрайта
			math::vec2f pos;					///> Позиция спрайта (в экранных координатах)
			math::vec2f size;					///> Размер спрайта (в экранных координатах)
			float spin;							///< Поворот
			ulong priority;			///> Приоритет отрисовки
			render::texture_ptr texture;			///> Адрес текстуры
			math::color color;					///> Цвет

			sprite();
			sprite( const math::vec2f& pos_, const math::vec2f& size_, 
				const math::color& color_ = 0xffffffff,render::texture_ptr tex_ = render::texture_ptr(), 
				float spin_ = 0, const math::rect& rect_ = math::rect(0, 0, 1, 1),			
				unsigned long priority = 0 );
		};

		class renderer2d
		{
		public:
			typedef std::vector<sprite> sprites_vector;
			typedef sprites_vector::iterator sprite_iter;

			renderer2d(device& dev);
			~renderer2d();

			inline sprites_vector& get_sprites() { return m_sprites; }

			void draw(const sprite& s);

			void render();
			void update();

		protected:
			bool check_size();
			void create_buffers();
			void fill_buffers();

		protected:
			sprites_vector m_sprites;

			vertex_declaration_ptr sprites_decl;			

			bool m_updated;							/// Были ли спрайты добавлены / удалены

			vertex_buffer_ptr m_vb;
			index_buffer_ptr m_ib;
			unsigned m_reserved_size;				/// Число спрайтов, под которое зарезервированы буферы

			device& m_device;
		};
	}
}