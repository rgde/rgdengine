#include "precompiled.h"

#include <rgde/math/animation_controller.h>


namespace math
{
	frame_animation::frame_animation( TiXmlNode* pXmlNode, frame_ptr frame)
	{
		m_paused = false;
		m_playing = false;
		m_looped = false;

		if ( pXmlNode != 0 )
		{
			load( pXmlNode );
		}

		m_frame = frame;
		m_speed = 1.0f;
		m_anim_time = 1;
		m_time = 0.0f;
		weight(1.0f);
	}

	bool frame_animation::load( TiXmlNode* pXmlNode )
	{
		TiXmlElement *elem = pXmlNode->FirstChildElement("animation");

		if ( elem == 0)
			return false;

		double time;
		elem->Attribute("time", &time );
		m_anim_time = (float)time;

		for (TiXmlElement* ev = elem->FirstChildElement("key"); 0 != ev; ev = ev->NextSiblingElement("key"))
		{
			ev->Attribute( "time", &time );
			TiXmlElement* child = 0;

			if ( child = ev->FirstChildElement("translation" ) )
			{
				double x,y,z;
				child->Attribute("x", &x);
				child->Attribute("y", &y);
				child->Attribute("z", &z);

				m_PosInterpolyator.add_key( (float)time, math::vec3f((float)x,(float)y,(float)z) );
			}

			if ( child = ev->FirstChildElement("rotation" ) )
			{
				double x,y,z;
				child->Attribute("x", &x);
				child->Attribute("y", &y);
				child->Attribute("z", &z);

				using math::Math::deg2Rad;

				math::vec3f v((float)deg2Rad(x), (float)deg2Rad(y), (float)deg2Rad(z));
				m_RotationInterpolyator.add_key( (float)time, v );
			}

			if ( child = ev->FirstChildElement("scale" ) )
			{
				double x,y,z;
				child->Attribute("x", &x);
				child->Attribute("y", &y);
				child->Attribute("z", &z);

				m_ScaleInterpolyator.add_key( (float)time, math::vec3f((float)x,(float)y,(float)z) );
			}

		}
		return true;
	}

	float frame_animation::weight() const
	{
		return m_fWeight;
	}

	void frame_animation::weight(float fWeight)
	{ 
		m_fWeight = fWeight;
		if((m_fWeight < 0.0f) || (m_fWeight > 1.0f)) 
			m_fWeight = 1.0f;
	}

	void frame_animation::update( float dt )
	{
		if ( !m_playing || !(m_anim_time > 0) || m_paused)
			return;

		m_time += dt*m_speed;

		if (m_time > m_anim_time)
			if ( m_looped )
				m_time -= (m_time/m_anim_time)*m_anim_time;
			else
				pause();

		update_transform();
	}

	void frame_animation::update_transform()
	{
		math::vec3f vec = m_RotationInterpolyator.get_value( m_time );
		math::EulerAngleXYZf ang( vec[ 0 ], vec[ 1 ], vec[ 2 ] );

		math::quatf q;
		math::set( q, ang );

		//if (m_frame)
		//{
		//
		//	m_frame->rotation( q );
		//	m_frame->position( m_PosInterpolyator.get_value( m_time/m_anim_time ) );
		//}

		vec = m_ScaleInterpolyator.get_value( m_time );
		if ( vec[ 0 ] > 1.5f )
		{
			int sdf = 0;
		}

		if (m_frame)
		{
			if(m_fWeight == 1.0f)
			{
				m_frame->scale( vec );
				m_frame->rotation( q );
				m_frame->position( m_PosInterpolyator.get_value( m_time ) );
			}
			else
			{
				float fInverseWeight = 1.0f - m_fWeight;
				m_frame->scale(m_frame->scale()*fInverseWeight + vec*m_fWeight );
				m_frame->rotation(m_frame->rotation()*fInverseWeight + q*m_fWeight  );
				m_frame->position(m_frame->position()*fInverseWeight +  m_PosInterpolyator.get_value( m_time/m_anim_time )*m_fWeight );
			}

			//Neonic: octree. 1 ставится, если было использовано вращение или увеличение/уменьшение. Иначе ставим 0.
			m_frame->update(1);
		}
	}

	void frame_animation::stop()
	{
		m_playing = false;
		m_paused = false;
		m_looped = false;
	}

	void frame_animation::pause()
	{
		m_paused = true;//!m_paused;
	}

	void frame_animation::start()
	{
		m_playing = true;
		m_paused = false;
	}
}