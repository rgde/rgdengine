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
 
				texture_ptr texture;  // текстура спрайта	
			};

			// описание вершины 2d-примитива
			struct prim_vertex		
			{
				math::vec4f	position;		
				math::vec2f	tex;			
			};

			// декларация вершины 2d-примитива
			vertex_element prim_vertex_desc[] = 
			{
				{0, 0,  vertex_element::float4, vertex_element::default_method, vertex_element::positiont, 0},
				{0, 16, vertex_element::float2,	vertex_element::default_method, vertex_element::texcoord,0},
				vertex_element::end_element
			};
		}
	}
}