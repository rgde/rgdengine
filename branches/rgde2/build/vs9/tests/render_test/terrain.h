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

			const vertex& get_vertex(int w, int l) const
			{
				assert(w<width && l < length);
				return *data[l*width+w];
			}



		}


	}
}