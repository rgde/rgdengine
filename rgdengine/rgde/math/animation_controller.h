#pragma once

#include <rgde/math/transform.h>
#include <rgde/math/interpolyator.h>

namespace math
{

	class frame_anim_controller
	{
		typedef math::interpolator<math::vec3f> position_interpolator;
		typedef math::interpolator<math::vec3f> rotation_interpolator;
		typedef math::interpolator<math::vec3f> scale_interpolator;

	public:
		frame_anim_controller( TiXmlNode* pXmlNode = 0, math::frame_ptr frame = frame_ptr());
		virtual ~frame_anim_controller(){}

		bool load( TiXmlNode* pXmlNode );

		float get_weight() const;
		void  set_weight(float fWeight);

		void atach( frame_ptr frame ) { m_spFrame = frame; }
		frame_ptr& get_frame() {return m_spFrame;} 

		inline void set_rate( float rate ) { m_fAnimationRate = rate; }
		inline float get_rate() const { return m_fAnimationRate; }

		inline void set_anim_time( float time ) { m_fAnimationTime = time; }

		void update( float dt );
		void updateMatrix();
		
		void  setCurrentTime(float fCurrentTime) { m_fCurrentTime = fCurrentTime; }
		float get_current_time() const { return m_fCurrentTime; }
		float getAnimationTime() const { return m_fAnimationTime; }

		void stop();
		void pause();
		void start();

		inline void set_looped( bool loop ) { m_bLooped = loop; }

		inline bool isPlaing() const { return m_bPlaying && !m_paused; }

		inline bool isPaused() const { return m_paused; }
		inline bool isLooped() const { return m_bLooped; }

		inline position_interpolator&	getPosInterpolyator() { return m_PosInterpolyator; }
		inline rotation_interpolator&	getRotationInterpolyator() { return m_RotationInterpolyator; }
		inline scale_interpolator&		getScaleInterpolyator() { return m_ScaleInterpolyator; }

	private:
		float m_fAnimationTime;
		float m_fCurrentTime;
		float m_fAnimationRate;
		float m_fWeight;

		math::frame_ptr m_spFrame;

		bool m_paused;
		bool m_bPlaying;
		bool m_bLooped;

		position_interpolator	m_PosInterpolyator;
		rotation_interpolator	m_RotationInterpolyator;
		scale_interpolator		m_ScaleInterpolyator;
	};
}