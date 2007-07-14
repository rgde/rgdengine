/*

@author PC
*/

#pragma once

/* Bone state class. Contains a state of a bone: position, orientation
   (rot) and scale. Also implements operators '+' and '*'. */
class CBoneState
{
public:

	CBoneState()
	{
		setScale(math::Vec3f(1.0f, 1.0f, 1.0f));
	}

	CBoneState(const math::Vec3f& pos, const math::Quatf& rot,
			   const math::Vec3f& scale)
	{
		setPos(pos);
		setRot(rot);
        setScale(scale);
	}

	inline CBoneState operator *(float w)
	{
		return CBoneState(getPos()*w, getRot()*w,
						  getScale()*w);
	}

	inline void operator *=(float w)
	{
		*this = *this*w;
	}

	inline CBoneState operator +(const CBoneState& state) const
	{
		return CBoneState(state.getPos() + getPos(),
			              state.getRot() + getRot(),
						  state.getScale() + getScale());
	}

	inline CBoneState operator -(const CBoneState& state) const
	{
		return CBoneState(state.getPos() - getPos(),
			              state.getRot() - getRot(),
						  state.getScale() - getScale());
	}

	inline void operator +=(const CBoneState& state)
	{
		*this = *this + state;
	}

	inline const math::Vec3f& getPos() const { return m_pos; }
	inline void               setPos(const math::Vec3f& pos)
	{
		m_pos = pos;
	}

	inline const math::Quatf& getRot() const { return m_rot; }
	inline void               setRot(const math::Quatf& rot)
	{
		m_rot = rot;
	}

	inline const math::Vec3f& getScale() const { return m_scale; }
	inline void               setScale(const math::Vec3f& scale)
	{
		m_scale = scale;
	}

private:

	math::Vec3f m_pos;
	math::Quatf m_rot;
	math::Vec3f m_scale;
};

io::IWriteStream& operator <<(io::IWriteStream& ws, const CBoneState& state);
io::IReadStream& operator >>(io::IReadStream& rs, CBoneState& state);