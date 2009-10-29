#include "precompiled.h"

#include <rgde/render/particles/emitter.h>
#include <rgde/io/io.h>

namespace particles
{

	//-----------------------------------------------------------------------------------
	void emitter::toStream (io::write_stream& wf) const
	{
		wf	<< (unsigned)m_type
			<< m_Transform;
	}

	//-----------------------------------------------------------------------------------
	void emitter::fromStream (io::read_stream& rf)
	{
		rf	>> m_Transform;
	}

}