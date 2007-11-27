#pragma once

#include "ArcBall.h"

namespace rgde
{
	class Camera
	{
	public:
		Camera()
		{
			m_ProjMatrix = math::MAT_IDENTITY44F;
			m_ViewMatrix = math::MAT_IDENTITY44F;
			m_rotx = m_roty = 0;
			m_distance = 5.0f;
		}
		~Camera(){}

		void setProjection(float fFovy, float fAspect, float fZNear, float fZFar)
		{
			m_ProjMatrix = math::makePerspectiveFovLH(fFovy, fAspect, fZNear, fZFar);
		}

		void lookAt(const math::vec3f& vEyePt, const math::vec3f& vLookatPt, const math::vec3f& vUpVec)
		{
			m_lookPt = vLookatPt;
			m_ViewMatrix = math::makeLookAt(vEyePt, vLookatPt, vUpVec);
			_asm nop;
		}

		math::mat44f getViewMatrix() 
		{
			math::vec3f cam_pos(-1 * m_distance, 0, 0);

			math::quatf quat = math::makeRot<math::quatf>(math::EulerAngleXYZf(m_rotx, m_roty, 0));
			
			math::mat44f rotation;
			math::set(rotation, quat);
			
			cam_pos = rotation * cam_pos;
			
			m_ViewMatrix = math::makeLookAt(cam_pos, math::vec3f(0,0,0), math::vec3f(0,1,0));
			
			return m_ViewMatrix;
		}

		const math::mat44f& getProjMatrix() const 
		{
			return m_ProjMatrix;
		}

		//движение
		void goForward(float delta)
		{
			math::mat44f trans = math::makeTrans<math::mat44f>(math::vec3f(0, 0, delta));

			m_ViewMatrix *= trans;

			m_distance += delta;
			m_distance = m_distance < 0 ? 5 : m_distance;
		}

		//вращение
		void rotateRight(float angle)
		{
			math::mat44f rot = math::makeRot<math::mat44f>(math::EulerAngleXYZf(0, angle, 0));
			math::invertFull(rot, rot);
			m_ViewMatrix = rot * m_ViewMatrix;
			m_roty -= angle;
		}

		void rotateUp(float angle)
		{
			math::mat44f rot = math::makeRot<math::mat44f>(math::EulerAngleXYZf(0, 0, angle));
			math::invertFull(rot, rot);
			m_ViewMatrix = rot * m_ViewMatrix;
			m_rotx -= angle;
		}

	protected:
		math::mat44f m_ProjMatrix;
		math::mat44f m_ViewMatrix;
		math::vec3f	 m_lookPt;
		float m_rotx, m_roty, m_distance;
	};

	class Application : window
	{
	public:
		Application();
		~Application();

		void run();

	protected:
		virtual core::windows::result wnd_proc(ushort message, uint wparam, long lparam );
		void resize_scene(unsigned int width, unsigned int height);
		bool do_events();

		const rgde::math::mat44f& getWorldMatrix() const 
		{
			return matWorld;
		}

	private:
		math::mat44f matWorld;
		ArcBall m_arc_ball;
		bool m_active;
		device m_device;
		rgde::render::font_ptr m_font;
		rgde::render::vertex_buffer_ptr m_vb;
		math::vec3f m_cam_pos;
		Camera m_camera;
	};
}