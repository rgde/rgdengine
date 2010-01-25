#pragma once

namespace rgde
{

	class terrain
	{
	public:
		terrain(unsigned int width, unsigned int length)
			: m_width(width),m_length(length)
		{
		}

		inline float get_height(unsigned int x, unsigned int z) const 
		{
			if(x<0 || x>=m_width || z<0 || z>=m_length )
				return 0.0f;

			return (float) rand()/RAND_MAX;
		}

		inline int get_width() const {return m_width;}
		inline int get_length()const {return m_length;}

	private:
		unsigned int m_width;
		unsigned int m_length;

	};
}