#pragma once

class WorldObject 
{
	public:
		WorldObject(){}
		
		const math::Vec3f& GetPosition() const {return m_pos;}
		void SetPosition(float x, float y, float z)
		{
			m_pos = math::Vec3f(x, y, z);
		}

		const math::Vec3f& GetVelocity() const {return m_vel;}
		void SetVelocity(float x, float y, float z)
		{
			m_vel = math::Vec3f(x, y, z);
		}

	protected:
		math::Vec3f	m_pos;
		math::Vec3f	m_vel;
};