#include "precompiled.h"

#include <rgde/math/camera.h>

#include <rgde/scene/scene.h>

namespace math
{
	base_camera::base_camera()
	{
	}

	base_camera::~base_camera()
	{

	}


	const matrix44f  base_camera::view_matrix()	const 
	{
		matrix44f m = frame::world_trasform();
		math::invert(m);
		return m;		
	}

	void base_camera::projection(float fovy, float aspect, float zn, float zf)
	{
		float hfov = fovy/2;
		float yScale = ::cos(hfov)/::sin(hfov);
		float xScale = yScale / aspect;

		m_projection.set(xScale,	0,			0,			0,
							0,		yScale,		0,			0, 
							0,		0,		zf/(zf-zn),		1,
							0,		0,		-zn*zf/(zf-zn), 0);

		math::transpose(m_projection);

		//Neonic: octree
		//setRadius(zn);
	}

	void base_camera::activate()
	{
		m_frustum.calculate(*this);

		//Neonic: octree. Этот код выполняет обновление положения камеры в дереве,
		// т.к. вызывается из выведенных камерах.
		//if(getRoot())
		//{
		//	move(position());
		//}
		//else
		//{
		//	// Neonic: next code appear repeatedly in sequence, cause camera (in tree) activation not instant
		//	makeSphere(position(),getRadius());
		//	// usually camera is moving (dynamic) object
		//	makeDynamic();
		//	scene::TheScene::get().inject(this);
		//}
	}
}