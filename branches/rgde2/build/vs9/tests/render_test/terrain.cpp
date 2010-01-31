#include "stdafx.h"
#include "terrain.h"

namespace rgde
{
	namespace terrain_container
	{
		terrain::terrain()
			: chunk_width(100)
			, chunk_length(100)
			, width(100)
			, length(100)
		{
			init();
		}
		terrain::terrain(uint w, uint l)
			: chunk_width(100)
			, chunk_length(100)
			, width(w)
			, length(l)
		{
			init();
		}

		terrain::terrain(uint w, uint l,uint cw, uint cl)
			: chunk_width(cw)
			, chunk_length(cl)
			, width(w)
			, length(l)
		{
			init();
		}

		void terrain::init()
		{
			chunk_data.resize(width*length, chunk(chunk_width,chunk_length));
		}
	}
}