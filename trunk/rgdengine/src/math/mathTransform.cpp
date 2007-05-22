#include "precompiled.h"

#include <rgde/math/mathTransform.h>

#include "../base/exception.h"
#include <rgde/base/lexical_cast.h>

#include <rgde/render/renderLines3d.h>

namespace math
{
	Matrix44f CFrame::makeTransformMatrix(const Point3f& pos, const Quatf& rot, const Vec3f& s)
	{
		math::Matrix44f rotation;
		math::setRot(rotation, rot);		

		math::Matrix44f translate;
		math::setTrans(translate, pos);

		math::Matrix44f scale;
		math::setScale(scale, s);

		return translate*rotation*scale;
	}

	Matrix44f CFrame::makeTransformMatrix(const Point3f& pos, const EulerAngleXYZf& rot, const Vec3f& s)
	{
		math::Quatf quat = math::make<Quatf, EulerAngleXYZf>(rot);
		return makeTransformMatrix(pos, quat, s);
	}

	CFrame::CFrame()
		: m_bIsNeedRecompute(false),
		  core::XmlNode<CFrame>("Frame"),
		  m_bNeedRecomputeGlobalMatrix(true),
		  m_scale(1.0f,1.0f,1.0f)
	{
		//CPropertyOwner::addProperty(new TProperty<math::Vec3f>(m_scale, "Scale"));
		//CPropertyOwner::addProperty(new TProperty<Point3f>(m_position, "Position", "Point"));
		//CPropertyOwner::addProperty(new TProperty<Quatf>(m_rotation, "Rotation", "Quaternion"));
	}

	CFrame::~CFrame()
	{
	}

	void CFrame::findFrames(const std::string& strTemplate, std::vector<PFrame>& container)
	{
		const std::string &strFrameName = getName();

		size_t nPos = strFrameName.find_first_of("_");
		if(nPos != -1)
		{
			size_t nBegin = strFrameName.find_first_not_of(" ");
			std::string strName = strFrameName.substr(nBegin, nPos - nBegin);

			if(strName == strTemplate)
				container.push_back(this);
		}

		for (math::CFrame::ChildrenList::const_iterator it = getChildren().begin(); it != getChildren().end(); it++)
			(*it)->findFrames(strTemplate, container);
	}

	void CFrame::setPosition(const Point3f& pos)
	{
		m_position = pos;
		m_bIsNeedRecompute = true;
	}

	void CFrame::setRotation(const Quatf& quat)
	{
		m_rotation = quat;
		m_bIsNeedRecompute = true;
	}

	void CFrame::lookAt(const Vec3f& vEyePt, const Vec3f& vLookatPt, const Vec3f& vUpVec)
	{	
		m_position = vEyePt;
		const math::Vec3f& up = vUpVec;
		math::Vec3f at = vLookatPt - vEyePt;

		{	
			using namespace math;
			Vec3f z = makeNormal<float>(at);
			Vec3f x = makeNormal<float>(makeCross(up, z));
			Vec3f y = makeCross<float>(z, x);

			Matrix33f mat = makeAxes<Matrix33f>(x, y, z);
			set(m_rotation, mat); 
		}

		m_bIsNeedRecompute = true;
	}

	void CFrame::setScale(const Vec3f& s)
	{
		m_scale = s;
		m_bIsNeedRecompute = true;
	}

	const Matrix44f & CFrame::getLocalTransform() const
	{
		computeLocalTransform();
		return m_localTransform;
	}

	const Matrix44f & CFrame::getFullTransform() const
	{
        computeFullTransform();
		return m_fullTransform;
	}

	void CFrame::debugDraw() const
	{
		const float l = 10.5f;
		math::Point3f p = getGlobalPosition();

		math::Point3f X = p + l * getLeftGlobal();
		math::Point3f Y = p + l * getUpGlobal();
		math::Point3f Z = p + l * getAtGlobal();

		render::CLine3dManager& pLine3dManager = render::Line3dManager::Get();
		pLine3dManager.addLine( p, X, math::Red );
		pLine3dManager.addLine( p, Y, math::Green );
		pLine3dManager.addLine( p, Z, math::Blue );
	}

	void CFrame::computeLocalTransform() const
	{
		if (!m_bIsNeedRecompute)
			return;

		math::Matrix44f rotation;
		math::setRot(rotation, m_rotation);		

		math::Matrix44f translate;
		math::setTrans(translate, m_position);
		
		math::Matrix44f scale;
		math::setScale(scale, m_scale);

		m_localTransform = translate * rotation * scale;

		m_bIsNeedRecompute = false;
		m_bNeedRecomputeGlobalMatrix = true;
	}

	void CFrame::computeFullTransform() const 
	{
		if (m_bIsNeedRecompute)
			computeLocalTransform();

		if (!m_bNeedRecomputeGlobalMatrix)
			return;

		computeLocalTransform();

		if (getParent())
			m_fullTransform = getParent()->getFullTransform() * m_localTransform;
		else
			m_fullTransform = m_localTransform;

		m_bNeedRecomputeGlobalMatrix = false;
	}

	void CFrame::onParentChange()
	{
		m_bIsNeedRecompute = true;
	}

	Point3f CFrame::getGlobalPosition() const 
	{
		computeFullTransform();
		const  Matrix44f &m	= m_fullTransform;
		return Point3f(m.mData[12], m.mData[13], m.mData[14]);
	}

	// left(right)  up          at
	//xaxis.x     yaxis.x     zaxis.x
	//xaxis.y     yaxis.y     zaxis.y
	//xaxis.z     yaxis.z     zaxis.z
	Vec3f CFrame::getUp() const 
	{
		computeLocalTransform();
		const Matrix44f &m= m_localTransform;
		return Vec3f(m[1][0], m[1][1], m[1][2]);
	}
	Vec3f CFrame::getAt() const 
	{
		computeLocalTransform();
		const Matrix44f &m= m_localTransform;
		return Vec3f(m[2][0], m[2][1], m[2][2]);
	}
	Vec3f CFrame::getLeft() const 
	{
		computeLocalTransform();
		const Matrix44f &m= m_localTransform;
		return Vec3f(m[0][0], m[0][1], m[0][2]);
	}

	Vec3f CFrame::getUpGlobal() const
	{
		computeFullTransform();
		Matrix44f &m = m_fullTransform;
		return Vec3f(m[1][0], m[1][1], m[1][2]);
	}

	Vec3f CFrame::getAtGlobal() const
	{
		computeFullTransform();
		Matrix44f &m = m_fullTransform;
		return Vec3f(m[2][0], m[2][1], m[2][2]);
	}

	Vec3f CFrame::getLeftGlobal() const
	{
		computeFullTransform();
		Matrix44f &m = m_fullTransform;
		return Vec3f(m[0][0], m[0][1], m[0][2]);
	}

	//Neonic: octree
	void CFrame::updateTree( bool NeedFullUpdate )
	{
		for (math::CFrame::ChildrenList::const_iterator it = getChildren().begin(); it != getChildren().end(); it++)
			(*it)->updateTree(NeedFullUpdate);
	};

	//-----------------------------------------------------------------------------------
	void CFrame::toStream(io::IWriteStream& wf) const
	{
		wf	<< m_scale
			<< m_position
			<< m_rotation;

		//// Сохраняем дочерние трансформации
		wf << (unsigned int)m_children.size();
		for( ChildrenList::const_iterator it = m_children.begin(); it != m_children.end(); it++ )
			(*it)->toStream( wf );
	}

	//-----------------------------------------------------------------------------------
	void CFrame::fromStream(io::IReadStream& rf)
	{
		rf	>> m_scale
			>> m_position
			>> m_rotation;

		m_bIsNeedRecompute = true;
		
		//// Читаем дочерние трансформации
		unsigned nChildren;
		rf >> nChildren;

		for(unsigned i = 0; i < nChildren; i++)
		{
			PFrame child = new CFrame;
			child->fromStream( rf );
			addChild( child );
		}
	}

	std::ostream& operator<<(std::ostream& out, const math::CFrame& f)
	{
		return out;
	}

	std::istream& operator>>(std::istream& in, math::CFrame& f)
	{
		return in;
	}
}