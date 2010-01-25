#pragma once

namespace rgde
{
	namespace terrain_container
	{
		struct vertex
		{
			vertex() : height(0)
			{
				norm[0] = .0f;
				norm[1] = .1f;
				norm[2] = .0f;
			}

			float height;
			math::vec3f norm;
		}
	
		class chunk
		{
		private:
			chunk(){};
			vertex* data;


		public:
			const uint width;
			const uint length;
			

			chunk(const uint w, const uint l) : width(w), length(l) 
			{
				data = new vertex[width*length];
			}

			const vertex& get_vertex(int x, int z) const
			{
				assert(x<width && z < length);
				return data[z*width+x];
			}
			inline float get_height_or_zero(unsigned int x, unsigned int z) const 
			{
				if(x<0 || x>=width || z<0 || z>=length )
					return 0.0f;
				return data[z*width+x];
			}
			void set_height(int x, int z, float height)
			{
				assert(x<width && z < length);
				data[z*width+x] = height;
				float h[6] = {.0f,.0f,.0f,.0f,.0f,.0f}
				if(z<length-1)	
				const float& h1 = get_height_or_zero( x,   z-1 );    
				const float& h2 = get_height_or_zero( x+1, z-1 );
				const float& h3 = get_height_or_zero( x+1, z   );
				const float& h4 = get_height_or_zero( x,   z+1 );
				const float& h5 = get_height_or_zero( x-1, z+1 );
				const float& h6 = get_height_or_zero( x-1, z   );
				data[z*width+x].norm[0] = h1-h2-3*h3-h4+h5+3*h6;
				data[z*width+x].norm[1] = 8;
				data[z*width+x].norm[2] = 3*h1+h2-h3-3*h4-h5+h6;
				math::normalize(data[z*width+x].norm);
			}

		}


	}
}