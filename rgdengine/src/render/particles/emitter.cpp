#include "precompiled.h"

#include <rgde/render/particles/emitter.h>
#include <rgde/io/io.h>

namespace particles
{

	//-----------------------------------------------------------------------------------
	void Emitter::toStream (io::IWriteStream& wf) const
	{
		wf	<< (unsigned)m_eType
			<< m_Transform;
	}

	//-----------------------------------------------------------------------------------
	void Emitter::fromStream (io::IReadStream& rf)
	{
		rf	>> m_Transform;
	}

}