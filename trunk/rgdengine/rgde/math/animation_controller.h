#pragma once

#include <rgde/math/transform.h>
#include <rgde/math/interpolyator.h>

namespace math
{
	class frame_animation
	{
		typedef math::interpolator<math::vec3f> position_interpolator;
		typedef math::interpolator<math::vec3f> rotation_interpolator;
		typedef math::interpolator<math::vec3f> scale_interpolator;

	public:
		frame_animation( TiXmlNode* pXmlNode = 0, math::frame_ptr frame = frame_ptr());
		virtual ~frame_animation(){}

		bool load( TiXmlNode* pXmlNode );

		float weight() const;
		void  weight(float w);

		void atach( frame_ptr frame ) { m_frame = frame; }
		frame_ptr& frame() {return m_frame;} 

		inline void speed( float rate ) { m_speed = rate; }
		inline float speed() const { return m_speed; }

		inline void anim_time( float time ) { m_anim_time = time; }

		void update( float dt );
		void update_transform();
		
		void  time(float t) { m_time = t; }
		float time() const { return m_time; }
		float anim_time() const { return m_anim_time; }

		void stop();
		void pause();
		void start();

		inline void looped( bool loop ) { m_looped = loop; }
		inline bool looped( ) const		{ return m_looped; }

		inline bool playing() const { return m_playing && !m_paused; }

		inline bool paused() const { return m_paused; }

		inline position_interpolator&	pos_interpolyator() { return m_PosInterpolyator; }
		inline rotation_interpolator&	rot_interpolyator() { return m_RotationInterpolyator; }
		inline scale_interpolator&		scale_interpolyator() { return m_ScaleInterpolyator; }

	private:
		float m_anim_time;
		float m_time;
		float m_speed;
		float m_fWeight;

		math::frame_ptr m_frame;

		bool m_paused;
		bool m_playing;
		bool m_looped;

		position_interpolator	m_PosInterpolyator;
		rotation_interpolator	m_RotationInterpolyator;
		scale_interpolator		m_ScaleInterpolyator;
	};
}