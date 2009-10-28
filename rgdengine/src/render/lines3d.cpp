#include "precompiled.h"

#include <rgde/render/lines3d.h>
#include <rgde/render/effect.h>
#include <rgde/render/device.h>


#include "../base/exception.h"

namespace render
{
	//-----------------------------------------------------------------------------------
	Line3dManager::Line3dManager(unsigned long uPriority)
		: render::rendererable(1000),
		  m_spEffect(effect::create("Line3dManager.fx")),
		  m_uPriority(uPriority),
		  m_Geometry(true)
	{
		//base::lmsg << "Line3dManager::Line3dManager()";
		m_pVertices = &(m_Geometry.lock());
		m_renderInfo.pRenderFunc = boost::bind(&Line3dManager::render, this);
	}

	//-----------------------------------------------------------------------------------
	void Line3dManager::render()
	{
		if (m_pVertices->size() == 0)
			return;

		math::camera_ptr pCamera	= render::TheDevice::get().get_curent_camera();

		if (!pCamera)
			return;

		const math::Matrix44f &mView = pCamera->getViewMatrix();
		const math::Matrix44f &mProj = pCamera->getProjMatrix();
		math::Matrix44f mLVP		 = mProj *mView;

		m_spEffect->getParams()["g_mLVP"]->set(mLVP);

		render::effect::ITechnique *pTechnique	= m_spEffect->findTechnique("Lines3d");
		if (0 == pTechnique)
			return;

		pTechnique->begin();
		m_spEffect->commitChanges();

		m_Geometry.unlock();

		size_t cPasses	= pTechnique->getPasses().size();

		for (unsigned iPass = 0; iPass < cPasses; ++iPass)
		{
			pTechnique->getPasses()[iPass]->begin();
			m_Geometry.render(PrimTypeLineList);
			pTechnique->getPasses()[iPass]->end();
		}

		pTechnique->end();

		// Сразу после отрисовки линий выносим все линии
		m_pVertices = &(m_Geometry.lock());
		m_pVertices->resize(0);
	}

	//-----------------------------------------------------------------------------------
	void Line3dManager::addLine(const math::Vec3f &vPoint1, const math::Vec3f &vPoint2, const math::Color &color)
	{
		m_pVertices->push_back(Point(vPoint1, color));
		m_pVertices->push_back(Point(vPoint2, color));
	}
	//-----------------------------------------------------------------------------------
	void Line3dManager::addBox(const math::Vec3f& size, const math::Color& color)
	{
		static const size_t nVerts = 8;
		static math::Vec3f box[nVerts];
		static bool isVertexesCreated	= false;

		if (!isVertexesCreated)
		{
			isVertexesCreated = true;

			box[0] = math::Vec3f(-1, 1, -1);
			box[1] = math::Vec3f(-1, 1, 1);
			box[2] = math::Vec3f(1, 1, 1);
			box[3] = math::Vec3f(1, 1, -1);

			box[4] = math::Vec3f(-1, -1, -1);
			box[5] = math::Vec3f(-1, -1, 1);
			box[6] = math::Vec3f(1, -1, 1);
			box[7] = math::Vec3f(1, -1, -1);
		}

		static math::Point3f size_box[nVerts];
		for (unsigned i = 0; i < nVerts; ++i)
		{
			const math::Vec3f &v= box[i];			
			size_box[i] = math::Point3f(v[0] * size[0], v[1] * size[1], v[2] * size[2]);
		}


		// Добавляем линии
		// up size
		addLine(size_box[0], size_box[1], color);
		addLine(size_box[1], size_box[2], color);
		addLine(size_box[2], size_box[3], color);
		addLine(size_box[3], size_box[0], color);

		//bottom size
		addLine(size_box[0 + 4], size_box[1 + 4], color);
		addLine(size_box[1 + 4], size_box[2 + 4], color);
		addLine(size_box[2 + 4], size_box[3 + 4], color);
		addLine(size_box[3 + 4], size_box[0 + 4], color);

		// connections between up and bottom
		addLine(size_box[0], size_box[0 + 4], color);
		addLine(size_box[1], size_box[1 + 4], color);
		addLine(size_box[2], size_box[2 + 4], color);
		addLine(size_box[3], size_box[3 + 4], color);
	}
	//-----------------------------------------------------------------------------------
	void Line3dManager::addBox(const math::AABoxf& box, const math::Color& color)
	{
		math::Point3f max = box.getMax();
		math::Point3f min = box.getMin();

		math::Point3f center = min + (max - min) / 2.0f;
		math::Point3f r		 = max - center;

		math::Point3f x(r[0], 0.0f, 0.0f);
		math::Point3f y(0.0f, r[1], 0.0f);
		math::Point3f z(0.0f, 0.0f, r[2]);

		math::Point3f& xm		= x;
		math::Point3f& ym		= y;
		math::Point3f& zm		= z;
		math::Point3f& centerm	= center;

		addLine(centerm + xm + ym + zm, centerm - xm + ym + zm, color);
		addLine(centerm + xm + ym - zm, centerm - xm + ym - zm, color);

		addLine(centerm + xm - ym + zm, centerm - xm - ym + zm, color);
		addLine(centerm + xm - ym - zm, centerm - xm - ym - zm, color);

		addLine(centerm + xm + ym + zm, centerm + xm - ym + zm, color);
		addLine(centerm + xm + ym - zm, centerm + xm - ym - zm, color);

		addLine(centerm - xm + ym + zm, centerm - xm - ym + zm, color);
		addLine(centerm - xm + ym - zm, centerm - xm - ym - zm, color);

		addLine(centerm + xm + ym + zm, centerm + xm + ym - zm, color);
		addLine(centerm + xm - ym + zm, centerm + xm - ym - zm, color);

		addLine(centerm - xm + ym + zm, centerm - xm + ym - zm, color);
		addLine(centerm - xm - ym + zm, centerm - xm - ym - zm, color);
	}
	//-----------------------------------------------------------------------------------
	void Line3dManager::addBox(const math::Matrix44f &m, const math::AABoxf &box, const math::Color &color)
	{
		math::Point3f max = box.getMax();
		math::Point3f min = box.getMin();

		math::Point3f center = min + (max - min) / 2.0f;
		math::Point3f r		 = max - center;

		math::Point3f x(r[0], 0.0f, 0.0f);
		math::Point3f y(0.0f, r[1], 0.0f);
		math::Point3f z(0.0f, 0.0f, r[2]);

		math::Matrix44f noTransTM	= m;
		noTransTM[0][3] = 0.0f;
		noTransTM[1][3] = 0.0f;
		noTransTM[2][3] = 0.0f;

		math::Point3f xm		= noTransTM *x;
		math::Point3f ym		= noTransTM *y;
		math::Point3f zm		= noTransTM *z;
		math::Point3f centerm	= m *center;

		addLine(centerm + xm + ym + zm, centerm - xm + ym + zm, color);
		addLine(centerm + xm + ym - zm, centerm - xm + ym - zm, color);

		addLine(centerm + xm - ym + zm, centerm - xm - ym + zm, color);
		addLine(centerm + xm - ym - zm, centerm - xm - ym - zm, color);

		addLine(centerm + xm + ym + zm, centerm + xm - ym + zm, color);
		addLine(centerm + xm + ym - zm, centerm + xm - ym - zm, color);

		addLine(centerm - xm + ym + zm, centerm - xm - ym + zm, color);
		addLine(centerm - xm + ym - zm, centerm - xm - ym - zm, color);

		addLine(centerm + xm + ym + zm, centerm + xm + ym - zm, color);
		addLine(centerm + xm - ym + zm, centerm + xm - ym - zm, color);

		addLine(centerm - xm + ym + zm, centerm - xm + ym - zm, color);
		addLine(centerm - xm - ym + zm, centerm - xm - ym - zm, color);
	}
	//-----------------------------------------------------------------------------------
	void Line3dManager::addBox(const math::Matrix44f &m, const math::Vec3f &size, const math::Color &color)
	{
		static const int nVerts = 8;
		static math::Vec3f box[nVerts];				// for size(x,y,z) = 1
		static bool isVertexesCreated	= false;

		if (!isVertexesCreated)
		{
			isVertexesCreated = true;

			box[0] = math::Vec3f(-1, 1, -1);
			box[1] = math::Vec3f(-1, 1, 1);
			box[2] = math::Vec3f(1, 1, 1);
			box[3] = math::Vec3f(1, 1, -1);

			box[4] = math::Vec3f(-1, -1, -1);
			box[5] = math::Vec3f(-1, -1, 1);
			box[6] = math::Vec3f(1, -1, 1);
			box[7] = math::Vec3f(1, -1, -1);
		}

		math::Point3f SizeBox[nVerts];
		for (unsigned i = 0; i < nVerts; ++i)
		{
			// by Korak
			const math::Vec3f &v= box[i];			
			SizeBox[i] = m * math::Point3f(v[0] * size[0], v[1] * size[1], v[2] * size[2]);
		}


		// Добавляем линии
		// up size
		addLine(SizeBox[0], SizeBox[1], color);
		addLine(SizeBox[1], SizeBox[2], color);
		addLine(SizeBox[2], SizeBox[3], color);
		addLine(SizeBox[3], SizeBox[0], color);

		//bottom size
		addLine(SizeBox[0 + 4], SizeBox[1 + 4], color);
		addLine(SizeBox[1 + 4], SizeBox[2 + 4], color);
		addLine(SizeBox[2 + 4], SizeBox[3 + 4], color);
		addLine(SizeBox[3 + 4], SizeBox[0 + 4], color);

		// connections between up and bottom
		addLine(SizeBox[0], SizeBox[0 + 4], color);
		addLine(SizeBox[1], SizeBox[1 + 4], color);
		addLine(SizeBox[2], SizeBox[2 + 4], color);
		addLine(SizeBox[3], SizeBox[3 + 4], color);
	}

	//-----------------------------------------------------------------------------------
	void Line3dManager::addDirection(const math::Matrix44f &m, const math::Point3f &dir, const math::Color &color)
	{
		math::Vec3f trans;
		math::setTrans(trans, m);
		addLine(trans, m * dir, color);
	}

	//-----------------------------------------------------------------------------------
	void Line3dManager::addSphere(const math::Matrix44f &m, float rad, int angle)
	{
		static math::Point3f circle[361];				// for radius = 1
		static math::Point3f circle2[2][361];
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

		math::Matrix44f m90z= math::makeRot<math::Matrix44f>(math::AxisAnglef(90.0f * 3.1415f / 180.0f, 0, 0, 1));
		math::Matrix44f m90y= math::makeRot<math::Matrix44f>(math::AxisAnglef(90.0f * 3.1415f / 180.0f, 0, 1, 0));
		m90y = m * m90y * m90z;
		m90z = m * m90z;

		for (int i = 0; i < 361; ++i)
		{
			if ((i > angle + 1) && (i < 359 - angle))
				continue;

			math::Point3f radCircle	= circle[i] * rad;
			circle2[0][i] = m90y * radCircle;
			circle2[1][i] = m90z * radCircle;
		}

		for (int i = 0; i < 360; ++i)
		{
			if (i > angle && i < 359 - angle)
				continue;

			addLine(circle2[0][i], circle2[0][i + 1], math::Green);
			addLine(circle2[1][i], circle2[1][i + 1], math::Green);
		}

		math::Vec3f zTrans;
		math::setTrans(zTrans, m90z);
		addLine(circle2[1][angle], zTrans, math::Green);
		addLine(zTrans, circle2[1][359 - angle], math::Green);

		math::Vec3f yTrans;
		math::setTrans(yTrans, m90y);
		addLine(circle2[0][angle], yTrans, math::Green);
		addLine(yTrans, circle2[0][359 - angle], math::Green);
	}
	//-----------------------------------------------------------------------------------
	void Line3dManager::addQuad(const math::Vec3f &center, const math::Vec2f &size, float fSpin)
	{
		const math::Matrix44f & mView = TheDevice::get().get_curent_camera()->getViewMatrix();

		math::Vec3f up	(mView.mData[0], mView.mData[4], mView.mData[8]);
		math::normalize(up);
		up *= size[0] * 0.5f;
		math::Vec3f right	(mView.mData[1], mView.mData[5], mView.mData[9]);
		math::normalize(right);
		right *= size[1] * 0.5f;

		float cosa		= cos(fSpin);
		float sina		= sin(fSpin);

		math::Vec3f p1	= center + (cosa - sina) * right + (-sina - cosa) * up;
		math::Vec3f p2	= center + (cosa + sina) * right + (-sina + cosa) * up;
		math::Vec3f p3	= center + (-cosa + sina) * right + (sina + cosa) * up;
		math::Vec3f p4	= center + (-cosa - sina) * right + (sina - cosa) * up;

		addLine(p1, p2, math::Blue);
		addLine(p2, p3, math::Blue);
		addLine(p3, p4, math::Blue);
		addLine(p4, p1, math::Blue);
	}
} //~ namespace utility