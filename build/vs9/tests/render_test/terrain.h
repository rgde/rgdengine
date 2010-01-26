#pragma once
#include "stdafx.h"

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
		};
	
		class chunk
		{
		private:
			uint width;
			uint length;
			std::vector<vertex> data;
			inline float get_height_or_zero(unsigned int x, unsigned int z) const 
			{
				if(x<0 || x>=width || z<0 || z>=length )
					return 0.0f;
				return data[z*width+x].height;
			}

			inline void init()
			{
				data.resize(width*length);
			}

		public:
			
			chunk()
				: width(256), length(256) 
			{
				init();
			};

			chunk(const uint w, const uint l) : width(w), length(l) 
			{
				init();
			}

			chunk& operator=(const chunk& other)
			{
				width = other.width;
				length = other.length;
				data = other.data;
				return *this;
			};

			inline const uint get_width() const{return width;};
			inline const uint get_length() const{return length;};
			const vertex& get_vertex(int x, int z) const
			{
				assert(x<width && z < length);
				return data[z*width+x];
			}
			void set_height(int x, int z, float height)
			{
				assert(x<width && z < length);
				data[z*width+x].height = height;
				float h[6] = {.0f,.0f,.0f,.0f,.0f,.0f};
				if(z<length-1)	
				h[0] = get_height_or_zero( x,   z-1 );    
				h[1] = get_height_or_zero( x+1, z-1 );
				h[2] = get_height_or_zero( x+1, z   );
				h[3] = get_height_or_zero( x,   z+1 );
				h[4] = get_height_or_zero( x-1, z+1 );
				h[5] = get_height_or_zero( x-1, z   );
				data[z*width+x].norm[0] = h[0]-h[1]-3*h[2]-h[3]+h[4]+3*h[5];
				data[z*width+x].norm[1] = 8;
				data[z*width+x].norm[2] = 3*h[0]+h[1]-h[2]-3*h[3]-h[4]+h[5];
				math::normalize(data[z*width+x].norm);
			}

		};

		class terrain
		{
		private:
			uint chunk_width;
			uint chunk_length;
			uint width;
			uint length;
			std::vector<chunk> chunk_data;

			void init();
		public:
			terrain();
			terrain(uint w, uint l);
			terrain(uint w, uint l,uint cw, uint cl);
			inline const uint get_width() const{return width;};
			inline const uint get_length() const{return length;};
			inline const uint get_chunk_width() const{return chunk_width;};
			inline const uint get_chunk_length() const{return chunk_length;};

			chunk& get_chunk(uint x, uint z)
			{
				assert(x < width && z < length);
				return chunk_data[z*width+x];
			};

		};
	}
}