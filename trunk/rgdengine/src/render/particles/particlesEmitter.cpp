#include "precompiled.h"

#include <rgde/render/particles/particlesEmitter.h>
#include <rgde/io/io.h>

namespace particles
{

	//-----------------------------------------------------------------------------------
	void IEmitter::toStream (io::IWriteStream& wf) const
	{
		wf	<< (unsigned)m_eType
			<< m_Transform;
	}

	//-----------------------------------------------------------------------------------
	void IEmitter::fromStream (io::IReadStream& rf)
	{
		rf	>> m_Transform;
	}

}