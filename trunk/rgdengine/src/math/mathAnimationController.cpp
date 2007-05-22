#include "precompiled.h"

#include <rgde/math/mathAnimationController.h>


namespace math
{
	CFrameAnimationController::CFrameAnimationController( TiXmlNode* pXmlNode, PFrame spFrame)
	{
		m_bPaused = false;
		m_bPlaying = false;
		m_bLooped = false;

		if ( pXmlNode != 0 )
		{
			load( pXmlNode );
		}

		m_spFrame = spFrame;
		m_fAnimationRate = 1.0f;
		m_fAnimationTime = 1;
		m_fCurrentTime = 0.0f;
		setWeight(1.0f);
	}

	bool CFrameAnimationController::load( TiXmlNode* pXmlNode )
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

				m_PosInterpolyator.addKey( (float)time, math::Vec3f((float)x,(float)y,(float)z) );
			}

			if ( child = ev->FirstChildElement("rotation" ) )
			{
				double x,y,z;
				child->Attribute("x", &x);
				child->Attribute("y", &y);
				child->Attribute("z", &z);

				using gmtl::Math::deg2Rad;

				math::Vec3f v((float)deg2Rad(x), (float)deg2Rad(y), (float)deg2Rad(z));
				m_RotationInterpolyator.addKey( (float)time, v );
			}

			if ( child = ev->FirstChildElement("scale" ) )
			{
				double x,y,z;
				child->Attribute("x", &x);
				child->Attribute("y", &y);
				child->Attribute("z", &z);

				m_ScaleInterpolyator.addKey( (float)time, math::Vec3f((float)x,(float)y,(float)z) );
			}

		}
	}

	float CFrameAnimationController::getWeight() const
	{
		return m_fWeight;
	}

	void CFrameAnimationController::setWeight(float fWeight)
	{ 
		m_fWeight = fWeight;
		if((m_fWeight < 0.0f) || (m_fWeight > 1.0f)) 
			m_fWeight = 1.0f;
	}

	void CFrameAnimationController::update( float dt )
	{
		if ( !m_bPlaying || !(m_fAnimationTime > 0) || m_bPaused)
			return;

		m_fCurrentTime += dt*m_fAnimationRate;

		if (m_fCurrentTime > m_fAnimationTime)
			if ( m_bLooped )
				m_fCurrentTime -= (m_fCurrentTime/m_fAnimationTime)*m_fAnimationTime;
			else
				pause();

		updateMatrix();
	}

	void CFrameAnimationController::updateMatrix()
	{
		math::Vec3f vec = m_RotationInterpolyator.getValue( m_fCurrentTime );
		math::EulerAngleXYZf ang( vec[ 0 ], vec[ 1 ], vec[ 2 ] );

		math::Quatf q;
		math::set( q, ang );

		//if (m_spFrame)
		//{
		//
		//	m_spFrame->setRotation( q );
		//	m_spFrame->setPosition( m_PosInterpolyator.getValue( m_fCurrentTime/m_fAnimationTime ) );
		//}

		vec = m_ScaleInterpolyator.getValue( m_fCurrentTime );
		if ( vec[ 0 ] > 1.5f )
		{
			int sdf = 0;
		}

		if (m_spFrame)
		{
			if(m_fWeight == 1.0f)
			{
				m_spFrame->setScale( vec );
				m_spFrame->setRotation( q );
				m_spFrame->setPosition( m_PosInterpolyator.getValue( m_fCurrentTime ) );
			}
			else
			{
				float fInverseWeight = 1.0f - m_fWeight;
				m_spFrame->setScale(m_spFrame->getScale()*fInverseWeight + vec*m_fWeight );
				m_spFrame->setRotation(m_spFrame->getRotation()*fInverseWeight + q*m_fWeight  );
				m_spFrame->setPosition(m_spFrame->getPosition()*fInverseWeight +  m_PosInterpolyator.getValue( m_fCurrentTime/m_fAnimationTime )*m_fWeight );
			}

			//Neonic: octree. 1 ставится, если было использовано вращение или увеличение/уменьшение. Иначе ставим 0.
			m_spFrame->updateTree(1);
		}
	}

	void CFrameAnimationController::stop()
	{
		m_bPlaying = false;
		m_bPaused = false;
		m_bLooped = false;
	}

	void CFrameAnimationController::pause()
	{
		m_bPaused = true;//!m_bPaused;
	}

	void CFrameAnimationController::start()
	{
		m_bPlaying = true;
		m_bPaused = false;
	}
}