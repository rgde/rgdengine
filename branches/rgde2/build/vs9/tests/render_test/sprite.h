#pragma once

namespace rgde
{
	namespace render
	{
		struct sprite
		{
			float x,y,w,h;				///> screen rect
			float uvx, uvy, uvw, uvh;	///> texture coordinates rect			
			float spin;					///< Поворот
			int color;					///> Цвет
			ulong priority;				///> Приоритет отрисовки
			render::texture_ptr texture;///> Адрес текстуры			

			sprite();
			sprite( const math::vec2f& pos_, const math::vec2f& size_, 
				math::color color_ = 0xffffffff,render::texture_ptr tex_ = render::texture_ptr(), 
				float spin_ = 0, const math::rect& rect_ = math::rect(0, 0, 1, 1),			
				unsigned long priority = 0 );
		};

		class canvas
		{
		public:
			typedef std::vector<sprite> sprites_vector;
			typedef sprites_vector::iterator sprite_iter;

			canvas(device& dev);
			~canvas();

			enum flags
			{
				f_none = 0,
				f_sort = 1,
				f_clear = 2,
			};

			//void begin(int flags = f_none | f_clear);
				void draw(const sprite& s);
				void draw(const math::rect& r, const math::color& c, texture_ptr t = texture_ptr());
				//void draw(const math::rect& r, const math::color& c);
				//void draw(float x, float y, float w, float h, int color);
				//void draw(float x, float y, float w, float h, int color, texture_ptr t);
			//void end() {update();}

			//void flush() {render();}

			void clear();

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
			size_t m_reserved_size;				/// Число спрайтов, под которое зарезервированы буферы

			device& m_device;

			math::vec2f m_scale;

			bool m_need_ib_update;
		};
	}
}