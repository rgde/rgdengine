#pragma once

namespace rgde
{
	namespace render
	{
		namespace primitives_2d
		{
			struct line_desc
			{
				line_desc();
			};

			struct sprite_desc
			{
				sprite_desc();

				math::vec2f pos;	  // позиция спрайта
				math::vec2f size;     // размеры спрайта
				math::rect tex_coord; // текстурные координаты спрайта

   				float spin;           // угол поворота спрайта
 
				math::color color;

				texture_ptr texture;  // текстура спрайта	
			};

			// описание вершины 2d-примитива
			struct prim_vertex		
			{
				math::vec4f	pos;	
				ulong color;
				math::vec2f	tex;			
			};
		}
	}
}