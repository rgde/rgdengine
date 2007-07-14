/*

@author PC
*/

#include "engine.h"
#include "BoneState.h"

io::IWriteStream& operator <<(io::IWriteStream& ws, const CBoneState& state)
{
	math::Vec3f pos = state.getPos(), scale = state.getScale();
	math::Quatf rot = state.getRot();
	ws<<pos;
	ws<<rot;
	ws<<scale;
	//ws<<state.getPos();
	//ws<<state.getRot();
	//ws<<state.getScale();
	return ws;
}


io::IReadStream& operator >>(io::IReadStream& rs, CBoneState& state)
{
	math::Vec3f pos, scale;
	math::Quatf rot;
	rs	>> pos;
	rs	>> rot;
	rs	>> scale;
	state.setPos(pos);
	state.setRot(rot);
	state.setScale(scale);
	return rs;
}