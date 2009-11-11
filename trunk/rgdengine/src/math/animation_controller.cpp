#include "precompiled.h"

#include <rgde/math/animation_controller.h>


namespace math
{
	frame_anim_controller::frame_anim_controller( TiXmlNode* pXmlNode, frame_ptr frame)
	{
		m_paused = false;
		m_bPlaying = false;
		m_bLooped = false;

		if ( pXmlNode != 0 )
		{
			load( pXmlNode );
		}

		m_frame = frame;
		m_fAnimationRate = 1.0f;
		m_fAnimationTime = 1;
		m_fCurrentTime = 0.0f;
		set_weight(1.0f);
	}

	bool frame_anim_controller::load( TiXmlNode* pXmlNode )
	{
		TiXmlElement *elem = pXmlNode->FirstChildElement("animation");

		if ( elem == 0)
			return false;

		double time;
		elem->Attribute("time", &time );
		m_fAnimationTime = (float)time;

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

	float frame_anim_controller::get_weight() const
	{
		return m_fWeight;
	}

	void frame_anim_controller::set_weight(float fWeight)
	{ 
		m_fWeight = fWeight;
		if((m_fWeight < 0.0f) || (m_fWeight > 1.0f)) 
			m_fWeight = 1.0f;
	}

	void frame_anim_controller::update( float dt )
	{
		if ( !m_bPlaying || !(m_fAnimationTime > 0) || m_paused)
			return;

		m_fCurrentTime += dt*m_fAnimationRate;

		if (m_fCurrentTime > m_fAnimationTime)
			if ( m_bLooped )
				m_fCurrentTime -= (m_fCurrentTime/m_fAnimationTime)*m_fAnimationTime;
			else
				pause();

		updateMatrix();
	}

	void frame_anim_controller::updateMatrix()
	{
		math::vec3f vec = m_RotationInterpolyator.getValue( m_fCurrentTime );
		math::EulerAngleXYZf ang( vec[ 0 ], vec[ 1 ], vec[ 2 ] );

		math::quatf q;
		math::set( q, ang );

		//if (m_frame)
		//{
		//
		//	m_frame->set_rot( q );
		//	m_frame->set_position( m_PosInterpolyator.getValue( m_fCurrentTime/m_fAnimationTime ) );
		//}

		vec = m_ScaleInterpolyator.getValue( m_fCurrentTime );
		if ( vec[ 0 ] > 1.5f )
		{
			int sdf = 0;
		}

		if (m_frame)
		{
			if(m_fWeight == 1.0f)
			{
				m_frame->set_scale( vec );
				m_frame->set_rot( q );
				m_frame->set_position( m_PosInterpolyator.getValue( m_fCurrentTime ) );
			}
			else
			{
				float fInverseWeight = 1.0f - m_fWeight;
				m_frame->set_scale(m_frame->get_scale()*fInverseWeight + vec*m_fWeight );
				m_frame->set_rot(m_frame->get_rot()*fInverseWeight + q*m_fWeight  );
				m_frame->set_position(m_frame->get_pos()*fInverseWeight +  m_PosInterpolyator.getValue( m_fCurrentTime/m_fAnimationTime )*m_fWeight );
			}

			//Neonic: octree. 1 ставится, если было использовано вращение или увеличение/уменьшение. Иначе ставим 0.
			m_frame->update(1);
		}
	}

	void frame_anim_controller::stop()
	{
		m_bPlaying = false;
		m_paused = false;
		m_bLooped = false;
	}

	void frame_anim_controller::pause()
	{
		m_paused = true;//!m_paused;
	}

	void frame_anim_controller::start()
	{
		m_bPlaying = true;
		m_paused = false;
	}
}