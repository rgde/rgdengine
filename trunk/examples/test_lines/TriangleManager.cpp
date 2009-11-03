#include <rgde/engine.h>

#include "TriangleManager.h"


namespace lines_test
{
	//-----------------------------------------------------------------------------------
	triangle_manager::triangle_manager( unsigned num_triangles )
	{
		m_triangles.resize(num_triangles);
		reset();
	}
	//-----------------------------------------------------------------------------------
	void triangle_manager::reset()
	{
		for( triangles_iter it = m_triangles.begin(); it != m_triangles.end(); ++it )
			it->reset();
	}
	//-----------------------------------------------------------------------------------
	void triangle_manager::update()
	{
		for( triangles_iter it = m_triangles.begin(); it != m_triangles.end(); ++it )
			it->update();
	}
	//-----------------------------------------------------------------------------------
	void triangle_manager::render()
	{
		for( triangles_iter it = m_triangles.begin(); it != m_triangles.end(); ++it )
			it->render();

		render::lines3d& line_manager = render::TheDevice::get().get_lines3d();
		line_manager.add_quad( math::Vec3f(0,0,0), math::Vec2f(40,40), 0 );
	}
}