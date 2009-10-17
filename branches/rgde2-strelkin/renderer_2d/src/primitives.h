#pragma once

namespace rgde
{
	namespace render
	{
		namespace primitive
		{
			struct line_desc
			{
				line_desc();

				//описание вершины линии
				struct vertex		
				{
					math::vec4f	position;		
					math::vec2f	tex;			
				};
			};

			struct sprite_desc
			{
				sprite_desc();

				math::vec2f pos;	  // позиция спрайта
				math::vec2f size;     // размеры спрайта
				math::rect tex_coord; // текстурные координаты спрайта

   				float spin;           // угол поворота спрайта
 
				texture_ptr texture;  // текстура спрайта	

				// описание вершины спрайта
				struct vertex		
				{
					math::vec4f	position;		
					math::vec2f	tex;			
				};
			};

			vertex_element sprites_vertex_desc[] = 
			{
				{0, 0,  vertex_element::float4, vertex_element::default_method, vertex_element::positiont, 0},
				{0, 16, vertex_element::float2,	vertex_element::default_method, vertex_element::texcoord,0},
				vertex_element::end_element
			};

			vertex_element lines_vertex_desc[] = 
			{
				{0, 0,  vertex_element::float4, vertex_element::default_method, vertex_element::positiont, 0},
				{0, 16, vertex_element::float2,	vertex_element::default_method, vertex_element::texcoord,0},
				vertex_element::end_element
			};
		}
	}
}