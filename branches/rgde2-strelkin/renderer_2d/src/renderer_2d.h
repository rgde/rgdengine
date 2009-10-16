#pragma once

namespace rgde
{
	namespace render
	{
		class renderer_2d
		{
		public:
			std::vector<primitive::line_desc> line_vector;
			std::vector<primitive::sprite_desc> sprite_vector;

			renderer_2d();
			~renderer_2d();

			void add_line(line_desc& line);
			void add_line(/* params*/);

			void add_sprite(sprite_desc& sprite);
			void add_sprite(/* params*/);

			void render_all();

		private:
			line_vector lines;
			sprite_vector sprites;
		};
	}
}