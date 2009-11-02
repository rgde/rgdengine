#include "precompiled.h"

#include <rgde/math/camera.h>

#include <rgde/scene/scene.h>

namespace math
{
	const Matrix44f  base_camera::get_view_matrix()	const 
	{
		Matrix44f m = frame::getFullTransform();
		math::invert(m);
		return m;		
	}

	void base_camera::set_projection(float fovy, float aspect, float zn, float zf)
	{
		float hfov = fovy/2;
		float yScale = ::cos(hfov)/::sin(hfov);
		float xScale = yScale / aspect;

		m_projection.set(xScale,	0,			0,			0,
							0,		yScale,		0,			0, 
							0,		0,		zf/(zf-zn),		1,
							0,		0,		-zn*zf/(zf-zn), 0);

		gmtl::transpose(m_projection);

		//Neonic: octree
		//setRadius(zn);
	}

	void base_camera::activate()
	{
		m_frustum.CalculateFrustum(*this);

		//Neonic: octree. Этот код выполняет обновление положения камеры в дереве,
		// т.к. вызывается из выведенных камерах.
		//if(getRoot())
		//{
		//	move(getPosition());
		//}
		//else
		//{
		//	// Neonic: next code appear repeatedly in sequence, cause camera (in tree) activation not instant
		//	makeSphere(getPosition(),getRadius());
		//	// usually camera is moving (dynamic) object
		//	makeDynamic();
		//	scene::TheScene::get().inject(this);
		//}
	}
}