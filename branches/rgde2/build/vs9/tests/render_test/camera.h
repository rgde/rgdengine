#pragma once

namespace rgde
{
	class camera
	{
	public:
		camera()
		{
			m_proj_matrix = math::MAT_IDENTITY44F;
			m_view_matrix = math::MAT_IDENTITY44F;
			m_rotx = m_roty = 0;
			m_distance = 5.0f;
		}
		~camera(){}

		void setProjection(float fov_y, float aspect, float znear, float zfar)
		{
			m_proj_matrix = math::make_perspective(fov_y, aspect, znear, zfar);
		}

		void lookAt(const math::vec3f& vEyePt, const math::vec3f& vLookatPt, const math::vec3f& vUpVec)
		{
			m_lookPt = vLookatPt;
			m_view_matrix = math::make_lookat(vEyePt, vLookatPt, vUpVec);
			_asm nop;
		}

		math::mat44f getViewMatrix() 
		{
			math::vec3f cam_pos(-1 * m_distance, 0, 0);

			math::quatf quat = math::makeRot<math::quatf>(math::EulerAngleXYZf(m_rotx, m_roty, 0));

			math::mat44f rotation;
			math::set(rotation, quat);

			cam_pos = rotation * cam_pos;

			m_view_matrix = math::make_lookat(cam_pos, math::vec3f(0,0,0), math::vec3f(0,1,0));

			return m_view_matrix;
		}

		const math::mat44f& getProjMatrix() const 
		{
			return m_proj_matrix;
		}

		//движение
		void goForward(float delta)
		{
			math::mat44f trans = math::makeTrans<math::mat44f>(math::vec3f(0, 0, delta));

			m_view_matrix *= trans;

			m_distance += delta;
			m_distance = m_distance < 0 ? 5 : m_distance;
		}

		//вращение
		void rotateRight(float angle)
		{
			math::mat44f rot = math::makeRot<math::mat44f>(math::EulerAngleXYZf(0, angle, 0));
			math::invertFull(rot, rot);
			m_view_matrix = rot * m_view_matrix;
			m_roty -= angle;
		}

		void rotateUp(float angle)
		{
			math::mat44f rot = math::makeRot<math::mat44f>(math::EulerAngleXYZf(0, 0, angle));
			math::invertFull(rot, rot);
			m_view_matrix = rot * m_view_matrix;
			m_rotx -= angle;
		}

	protected:
		math::mat44f m_proj_matrix;
		math::mat44f m_view_matrix;
		math::vec3f	 m_lookPt;
		float m_rotx, m_roty, m_distance;
	};
}