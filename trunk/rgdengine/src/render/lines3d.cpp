#include "precompiled.h"

#include <rgde/render/lines3d.h>
#include <rgde/render/effect.h>
#include <rgde/render/render_device.h>


#include "../base/exception.h"

namespace render
{
	//-----------------------------------------------------------------------------------
	lines3d::lines3d(unsigned long priority)
		: render::rendererable(1000),
		  m_effect(effect::create("Line3dManager.fx")),
		  m_priority(priority),
		  m_geometry(true)
	{
		//base::lmsg << "lines3d::lines3d()";
		m_vertices = &(m_geometry.lock());
		m_renderInfo.render_func = boost::bind(&lines3d::render, this);
	}

	//-----------------------------------------------------------------------------------
	void lines3d::render()
	{
		if (m_vertices->size() == 0)
			return;

		math::camera_ptr camera	= render::render_device::get().get_camera();

		if (!camera)
			return;

		const math::matrix44f &mView = camera->get_view_matrix();
		const math::matrix44f &mProj = camera->get_proj_matrix();
		math::matrix44f mLVP		 = mProj *mView;

		m_effect->get_params()["g_mLVP"]->set(mLVP);

		render::effect::technique *pTechnique	= m_effect->find_technique("Lines3d");
		if (0 == pTechnique)
			return;

		pTechnique->begin();
		m_effect->commit_changes();

		m_geometry.unlock();

		size_t cPasses	= pTechnique->get_passes().size();

		for (unsigned iPass = 0; iPass < cPasses; ++iPass)
		{
			pTechnique->get_passes()[iPass]->begin();
			m_geometry.render(LineList);
			pTechnique->get_passes()[iPass]->end();
		}

		pTechnique->end();

		// Сразу после отрисовки линий выносим все линии
		m_vertices = &(m_geometry.lock());
		m_vertices->resize(0);
	}

	//-----------------------------------------------------------------------------------
	void lines3d::add_line(const math::vec3f &point1, const math::vec3f &point2, const math::Color &color)
	{
		m_vertices->push_back(Point(point1, color));
		m_vertices->push_back(Point(point2, color));
	}
	//-----------------------------------------------------------------------------------
	void lines3d::add_box(const math::vec3f& size, const math::Color& color)
	{
		static const size_t nVerts = 8;
		static math::vec3f box[nVerts];
		static bool isVertexesCreated	= false;

		if (!isVertexesCreated)
		{
			isVertexesCreated = true;

			box[0] = math::vec3f(-1, 1, -1);
			box[1] = math::vec3f(-1, 1, 1);
			box[2] = math::vec3f(1, 1, 1);
			box[3] = math::vec3f(1, 1, -1);

			box[4] = math::vec3f(-1, -1, -1);
			box[5] = math::vec3f(-1, -1, 1);
			box[6] = math::vec3f(1, -1, 1);
			box[7] = math::vec3f(1, -1, -1);
		}

		static math::point3f size_box[nVerts];
		for (unsigned i = 0; i < nVerts; ++i)
		{
			const math::vec3f &v= box[i];			
			size_box[i] = math::point3f(v[0] * size[0], v[1] * size[1], v[2] * size[2]);
		}


		// Добавляем линии
		// up size
		add_line(size_box[0], size_box[1], color);
		add_line(size_box[1], size_box[2], color);
		add_line(size_box[2], size_box[3], color);
		add_line(size_box[3], size_box[0], color);

		//bottom size
		add_line(size_box[0 + 4], size_box[1 + 4], color);
		add_line(size_box[1 + 4], size_box[2 + 4], color);
		add_line(size_box[2 + 4], size_box[3 + 4], color);
		add_line(size_box[3 + 4], size_box[0 + 4], color);

		// connections between up and bottom
		add_line(size_box[0], size_box[0 + 4], color);
		add_line(size_box[1], size_box[1 + 4], color);
		add_line(size_box[2], size_box[2 + 4], color);
		add_line(size_box[3], size_box[3 + 4], color);
	}
	//-----------------------------------------------------------------------------------
	void lines3d::add_box(const math::aaboxf& box, const math::Color& color)
	{
		math::point3f max = box.getMax();
		math::point3f min = box.getMin();

		math::point3f center = min + (max - min) / 2.0f;
		math::point3f r		 = max - center;

		math::point3f x(r[0], 0.0f, 0.0f);
		math::point3f y(0.0f, r[1], 0.0f);
		math::point3f z(0.0f, 0.0f, r[2]);

		math::point3f& xm		= x;
		math::point3f& ym		= y;
		math::point3f& zm		= z;
		math::point3f& centerm	= center;

		add_line(centerm + xm + ym + zm, centerm - xm + ym + zm, color);
		add_line(centerm + xm + ym - zm, centerm - xm + ym - zm, color);

		add_line(centerm + xm - ym + zm, centerm - xm - ym + zm, color);
		add_line(centerm + xm - ym - zm, centerm - xm - ym - zm, color);

		add_line(centerm + xm + ym + zm, centerm + xm - ym + zm, color);
		add_line(centerm + xm + ym - zm, centerm + xm - ym - zm, color);

		add_line(centerm - xm + ym + zm, centerm - xm - ym + zm, color);
		add_line(centerm - xm + ym - zm, centerm - xm - ym - zm, color);

		add_line(centerm + xm + ym + zm, centerm + xm + ym - zm, color);
		add_line(centerm + xm - ym + zm, centerm + xm - ym - zm, color);

		add_line(centerm - xm + ym + zm, centerm - xm + ym - zm, color);
		add_line(centerm - xm - ym + zm, centerm - xm - ym - zm, color);
	}
	//-----------------------------------------------------------------------------------
	void lines3d::add_box(const math::matrix44f &m, const math::aaboxf &box, const math::Color &color)
	{
		math::point3f max = box.getMax();
		math::point3f min = box.getMin();

		math::point3f center = min + (max - min) / 2.0f;
		math::point3f r		 = max - center;

		math::point3f x(r[0], 0.0f, 0.0f);
		math::point3f y(0.0f, r[1], 0.0f);
		math::point3f z(0.0f, 0.0f, r[2]);

		math::matrix44f noTransTM	= m;
		noTransTM[0][3] = 0.0f;
		noTransTM[1][3] = 0.0f;
		noTransTM[2][3] = 0.0f;

		math::point3f xm		= noTransTM *x;
		math::point3f ym		= noTransTM *y;
		math::point3f zm		= noTransTM *z;
		math::point3f centerm	= m *center;

		add_line(centerm + xm + ym + zm, centerm - xm + ym + zm, color);
		add_line(centerm + xm + ym - zm, centerm - xm + ym - zm, color);

		add_line(centerm + xm - ym + zm, centerm - xm - ym + zm, color);
		add_line(centerm + xm - ym - zm, centerm - xm - ym - zm, color);

		add_line(centerm + xm + ym + zm, centerm + xm - ym + zm, color);
		add_line(centerm + xm + ym - zm, centerm + xm - ym - zm, color);

		add_line(centerm - xm + ym + zm, centerm - xm - ym + zm, color);
		add_line(centerm - xm + ym - zm, centerm - xm - ym - zm, color);

		add_line(centerm + xm + ym + zm, centerm + xm + ym - zm, color);
		add_line(centerm + xm - ym + zm, centerm + xm - ym - zm, color);

		add_line(centerm - xm + ym + zm, centerm - xm + ym - zm, color);
		add_line(centerm - xm - ym + zm, centerm - xm - ym - zm, color);
	}
	//-----------------------------------------------------------------------------------
	void lines3d::add_box(const math::matrix44f &m, const math::vec3f &size, const math::Color &color)
	{
		static const int nVerts = 8;
		static math::vec3f box[nVerts];				// for size(x,y,z) = 1
		static bool isVertexesCreated	= false;

		if (!isVertexesCreated)
		{
			isVertexesCreated = true;

			box[0] = math::vec3f(-1, 1, -1);
			box[1] = math::vec3f(-1, 1, 1);
			box[2] = math::vec3f(1, 1, 1);
			box[3] = math::vec3f(1, 1, -1);

			box[4] = math::vec3f(-1, -1, -1);
			box[5] = math::vec3f(-1, -1, 1);
			box[6] = math::vec3f(1, -1, 1);
			box[7] = math::vec3f(1, -1, -1);
		}

		math::point3f SizeBox[nVerts];
		for (unsigned i = 0; i < nVerts; ++i)
		{
			// by Korak
			const math::vec3f &v= box[i];			
			SizeBox[i] = m * math::point3f(v[0] * size[0], v[1] * size[1], v[2] * size[2]);
		}


		// Добавляем линии
		// up size
		add_line(SizeBox[0], SizeBox[1], color);
		add_line(SizeBox[1], SizeBox[2], color);
		add_line(SizeBox[2], SizeBox[3], color);
		add_line(SizeBox[3], SizeBox[0], color);

		//bottom size
		add_line(SizeBox[0 + 4], SizeBox[1 + 4], color);
		add_line(SizeBox[1 + 4], SizeBox[2 + 4], color);
		add_line(SizeBox[2 + 4], SizeBox[3 + 4], color);
		add_line(SizeBox[3 + 4], SizeBox[0 + 4], color);

		// connections between up and bottom
		add_line(SizeBox[0], SizeBox[0 + 4], color);
		add_line(SizeBox[1], SizeBox[1 + 4], color);
		add_line(SizeBox[2], SizeBox[2 + 4], color);
		add_line(SizeBox[3], SizeBox[3 + 4], color);
	}

	//-----------------------------------------------------------------------------------
	void lines3d::add_arrow(const math::matrix44f &m, const math::point3f &dir, const math::Color &color)
	{
		math::vec3f trans;
		math::setTrans(trans, m);
		add_line(trans, m * dir, color);
	}

	//-----------------------------------------------------------------------------------
	void lines3d::add_sphere(const math::matrix44f &m, float rad, int angle)
	{
		static math::point3f circle[361];				// for radius = 1
		static math::point3f circle2[2][361];
		static bool isVertexesCreated	= false;

		if (!isVertexesCreated)
		{
			isVertexesCreated = true;
			for (int i = 0; i < 361; ++i)
			{
				circle[i][0] = cosf(i * 3.1415f / 180.0f);
				circle[i][1] = sinf(i * 3.1415f / 180.0f);
				circle[i][2] = 0;
			}
		}

		math::matrix44f m90z= math::makeRot<math::matrix44f>(math::AxisAnglef(90.0f * 3.1415f / 180.0f, 0, 0, 1));
		math::matrix44f m90y= math::makeRot<math::matrix44f>(math::AxisAnglef(90.0f * 3.1415f / 180.0f, 0, 1, 0));
		m90y = m * m90y * m90z;
		m90z = m * m90z;

		for (int i = 0; i < 361; ++i)
		{
			if ((i > angle + 1) && (i < 359 - angle))
				continue;

			math::point3f radCircle	= circle[i] * rad;
			circle2[0][i] = m90y * radCircle;
			circle2[1][i] = m90z * radCircle;
		}

		for (int i = 0; i < 360; ++i)
		{
			if (i > angle && i < 359 - angle)
				continue;

			add_line(circle2[0][i], circle2[0][i + 1], math::Green);
			add_line(circle2[1][i], circle2[1][i + 1], math::Green);
		}

		math::vec3f zTrans;
		math::setTrans(zTrans, m90z);
		add_line(circle2[1][angle], zTrans, math::Green);
		add_line(zTrans, circle2[1][359 - angle], math::Green);

		math::vec3f yTrans;
		math::setTrans(yTrans, m90y);
		add_line(circle2[0][angle], yTrans, math::Green);
		add_line(yTrans, circle2[0][359 - angle], math::Green);
	}
	//-----------------------------------------------------------------------------------
	void lines3d::add_quad(const math::vec3f &center, const math::vec2f &size, float spin)
	{
		const math::matrix44f & mView = render_device::get().get_camera()->get_view_matrix();

		math::vec3f up	(mView.mData[0], mView.mData[4], mView.mData[8]);
		math::normalize(up);
		up *= size[0] * 0.5f;
		math::vec3f right	(mView.mData[1], mView.mData[5], mView.mData[9]);
		math::normalize(right);
		right *= size[1] * 0.5f;

		float cosa		= cos(spin);
		float sina		= sin(spin);

		math::vec3f p1	= center + (cosa - sina) * right + (-sina - cosa) * up;
		math::vec3f p2	= center + (cosa + sina) * right + (-sina + cosa) * up;
		math::vec3f p3	= center + (-cosa + sina) * right + (sina + cosa) * up;
		math::vec3f p4	= center + (-cosa - sina) * right + (sina - cosa) * up;

		add_line(p1, p2, math::Blue);
		add_line(p2, p3, math::Blue);
		add_line(p3, p4, math::Blue);
		add_line(p4, p1, math::Blue);
	}
} //~ namespace utility