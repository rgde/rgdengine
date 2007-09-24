#pragma once

#include "ArcBall.h"
#include <d3dx9.h>

namespace rgde
{
	namespace render
	{
		namespace graphics2d
		{
			class bather
			{
			public:
				virtual ~bather(){}

			};

						
		}
	}
}


class Camera
{
public:
	Camera()
	{
		D3DXMatrixIdentity(&m_ProjMatrix);
		D3DXMatrixIdentity(&m_ViewMatrix);

		m_rotx = m_roty = 0;
		m_distance = 5.0f;
	}
	~Camera(){}

	void setProjection(float fFovy, float fAspect, float fZNear, float fZFar)
	{
		D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, fFovy, fAspect, fZNear, fZFar );
	}

	void lookAt(const D3DXVECTOR3& vEyePt, const D3DXVECTOR3& vLookatPt, const D3DXVECTOR3& vUpVec)
	{
		m_lookPt = vLookatPt;
		D3DXMatrixLookAtLH(&m_ViewMatrix, &vEyePt, &vLookatPt, &vUpVec);
	}

	D3DXMATRIX getViewMatrix() 
	{
		D3DXVECTOR3 cam_pos(-1 * m_distance, 0, 0);

		D3DXQUATERNION quat;
		D3DXQuaternionRotationYawPitchRoll(&quat, m_roty, 0, m_rotx);

		D3DXMATRIX rotation;
		D3DXMatrixRotationQuaternion(&rotation, &quat);

		D3DXVec3TransformCoord(&cam_pos, &cam_pos, &rotation);
		
		D3DXMatrixLookAtLH(&m_ViewMatrix, &cam_pos, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));

		return m_ViewMatrix;
	}
	const D3DXMATRIX& getProjMatrix() const {return m_ProjMatrix;}

	//движение
	void goForward(float delta)
	{
		D3DXMATRIX trans;
		D3DXMatrixTranslation(&trans, 0, 0, delta);
		m_ViewMatrix *= trans;

		m_distance += delta;
		m_distance = m_distance < 0 ? 5 : m_distance;
	}

	//вращение
	void rotateRight(float angle)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationY(&rot, angle);

		D3DXMatrixInverse(&rot, NULL, &rot);
		m_ViewMatrix = rot * m_ViewMatrix;

		m_roty -= angle;
	}

	void rotateUp(float angle)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationZ(&rot, angle);

		//D3DXMATRIX inv_view;
		D3DXMatrixInverse(&rot, NULL, &rot);
		m_ViewMatrix = rot * m_ViewMatrix;

		m_rotx -= angle;

	}

protected:
	D3DXMATRIX		m_ProjMatrix;
	D3DXMATRIX		m_ViewMatrix;
	D3DXVECTOR3		m_lookPt;

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

private:
	ArcBall m_arc_ball;
	bool m_active;
	device m_device;
	font m_font;
	IDirect3DVertexBuffer9 *pVertexBuffer;
	D3DXVECTOR3 m_cam_pos;
	Camera m_camera;
};
