#pragma once

#include <d3dx9math.h>

class ArcBall
{
public:
	ArcBall(float width, float height);
	~ArcBall() {}

	void set_bounds(float width, float height)
	{
		assert((width > 1.0f) && (height > 1.0f));

		//Set adjustment factor for width/height
		this->AdjustWidth  = 1.0f / ((width  - 1.0f) * 0.5f);
		this->AdjustHeight = 1.0f / ((height - 1.0f) * 0.5f);
	}

	void click(int x, int y);
	void drag(int x, int y);

	const D3DXQUATERNION& get_quaternion() const {return m_quaternion;}
	const D3DXMATRIX& get_matrix() const {return m_matrix;}

protected:
	void mapToSphere(int x, int y, D3DXVECTOR3& new_vec) const;

protected:
	D3DXQUATERNION m_quaternion;
	D3DXMATRIX m_matrix;

	D3DXVECTOR3   StVec;          //Saved click vector
	D3DXVECTOR3   EnVec;          //Saved drag vector

	float     AdjustWidth;    //Mouse bounds width
	float     AdjustHeight;   //Mouse bounds height
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
	bool m_active;
	device m_device;
	font m_font;
	IDirect3DVertexBuffer9 *pVertexBuffer;
	D3DXVECTOR3 m_cam_pos;
};
