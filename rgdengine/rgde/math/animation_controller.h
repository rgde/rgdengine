#pragma once

#include <rgde/math/transform.h>
#include <rgde/math/interpolyator.h>

namespace math
{

	class FrameAnimationController
	{
		typedef math::TInterpolator<math::Vec3f> PositionInterpolyator;
		typedef math::TInterpolator<math::Vec3f> RotationInterpolyator;
		typedef math::TInterpolator<math::Vec3f> ScaleInterpolyator;

	public:
		FrameAnimationController( TiXmlNode* pXmlNode = 0, math::PFrame spFrame = PFrame());
		virtual ~FrameAnimationController(){}

		bool load( TiXmlNode* pXmlNode );

		float getWeight() const;
		void  setWeight(float fWeight);

		void atachToFrame( PFrame spFrame ) { m_spFrame = spFrame; }
		PFrame& getFrame() {return m_spFrame;} 

		inline void setRate( float rate ) { m_fAnimationRate = rate; }
		inline float getRate() const { return m_fAnimationRate; }

		inline void SetAnimationTime( float time ) { m_fAnimationTime = time; }

		void update( float dt );
		void updateMatrix();
		
		void  setCurrentTime(float fCurrentTime) { m_fCurrentTime = fCurrentTime; }
		float getCurrentTime() const { return m_fCurrentTime; }
		float getAnimationTime() const { return m_fAnimationTime; }

		void stop();
		void pause();
		void start();

		inline void setLooped( bool loop ) { m_bLooped = loop; }

		inline bool isPlaing() const { return m_bPlaying && !m_bPaused; }

		inline bool isPaused() const { return m_bPaused; }
		inline bool isLooped() const { return m_bLooped; }

		inline PositionInterpolyator&	getPosInterpolyator() { return m_PosInterpolyator; }
		inline RotationInterpolyator&	getRotationInterpolyator() { return m_RotationInterpolyator; }
		inline ScaleInterpolyator&		getScaleInterpolyator() { return m_ScaleInterpolyator; }

	private:
		float m_fAnimationTime;
		float m_fCurrentTime;
		float m_fAnimationRate;
		float m_fWeight;

		math::PFrame m_spFrame;

		bool m_bPaused;
		bool m_bPlaying;
		bool m_bLooped;

		PositionInterpolyator	m_PosInterpolyator;
		RotationInterpolyator	m_RotationInterpolyator;
		ScaleInterpolyator		m_ScaleInterpolyator;
	};
}