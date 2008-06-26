#include "precompiled.h"

#include <rgde/math/transform.h>

#include "../base/exception.h"
#include <rgde/base/lexical_cast.h>

#include <rgde/render/lines3d.h>

namespace math
{
	Matrix44f Frame::makeTransformMatrix(const Point3f& pos, const Quatf& rot, const Vec3f& s)
	{
		math::Matrix44f rotation;
		math::setRot(rotation, rot);		

		math::Matrix44f translate;
		math::setTrans(translate, pos);

		math::Matrix44f scale;
		math::setScale(scale, s);

		return translate*rotation*scale;
	}

	Matrix44f Frame::makeTransformMatrix(const Point3f& pos, const EulerAngleXYZf& rot, const Vec3f& s)
	{
		math::Quatf quat = math::make<Quatf, EulerAngleXYZf>(rot);
		return makeTransformMatrix(pos, quat, s);
	}

	Frame::Frame()
		: m_bIsNeedRecompute(false),
		  core::XmlNode<Frame>("Frame"),
		  m_bNeedRecomputeGlobalMatrix(true),
		  m_scale(1.0f,1.0f,1.0f)
	{
		//PropertyOwner::addProperty(new TProperty<math::Vec3f>(m_scale, "Scale"));
		//PropertyOwner::addProperty(new TProperty<Point3f>(m_position, "Position", "Point"));
		//PropertyOwner::addProperty(new TProperty<Quatf>(m_rotation, "Rotation", "Quaternion"));
	}

	Frame::~Frame()
	{
	}

	void Frame::findFrames(const std::string& strTemplate, std::vector<PFrame>& container)
	{
		const std::string &strFrameName = getName();

		size_t nPos = strFrameName.find_first_of("_");
		if(nPos != -1)
		{
			size_t nBegin = strFrameName.find_first_not_of(" ");
			std::string name = strFrameName.substr(nBegin, nPos - nBegin);

			if(name == strTemplate)
				container.push_back(this);
		}

		for (math::Frame::ChildrenList::const_iterator it = getChildren().begin(); it != getChildren().end(); it++)
			(*it)->findFrames(strTemplate, container);
	}

	void Frame::setPosition(const Point3f& pos)
	{
		m_position = pos;
		m_bIsNeedRecompute = true;
	}

	void Frame::setRotation(const Quatf& quat)
	{
		m_rotation = quat;
		m_bIsNeedRecompute = true;
	}

	void Frame::lookAt(const Vec3f& vEyePt, const Vec3f& vLookatPt, const Vec3f& vUpVec)
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

	void Frame::setScale(const Vec3f& s)
	{
		m_scale = s;
		m_bIsNeedRecompute = true;
	}

	const Matrix44f & Frame::getLocalTransform() const
	{
		computeLocalTransform();
		return m_localTransform;
	}

	const Matrix44f & Frame::getFullTransform() const
	{
        computeFullTransform();
		return m_fullTransform;
	}

	void Frame::debugDraw() const
	{
		const float l = 10.5f;
		math::Point3f p = getGlobalPosition();

		math::Point3f X = p + l * getLeftGlobal();
		math::Point3f Y = p + l * getUpGlobal();
		math::Point3f Z = p + l * getAtGlobal();

		render::Line3dManager& line_manager = render::TheLine3dManager::Get();
		line_manager.addLine( p, X, math::Red );
		line_manager.addLine( p, Y, math::Green );
		line_manager.addLine( p, Z, math::Blue );
	}

	void Frame::computeLocalTransform() const
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

	void Frame::computeFullTransform() const 
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

	void Frame::onParentChange()
	{
		m_bIsNeedRecompute = true;
	}

	Point3f Frame::getGlobalPosition() const 
	{
		computeFullTransform();
		const  Matrix44f &m	= m_fullTransform;
		return Point3f(m.mData[12], m.mData[13], m.mData[14]);
	}

	// left(right)  up          at
	//xaxis.x     yaxis.x     zaxis.x
	//xaxis.y     yaxis.y     zaxis.y
	//xaxis.z     yaxis.z     zaxis.z
	Vec3f Frame::getUp() const 
	{
		computeLocalTransform();
		const Matrix44f &m= m_localTransform;
		return Vec3f(m[1][0], m[1][1], m[1][2]);
	}
	Vec3f Frame::getAt() const 
	{
		computeLocalTransform();
		const Matrix44f &m= m_localTransform;
		return Vec3f(m[2][0], m[2][1], m[2][2]);
	}
	Vec3f Frame::getLeft() const 
	{
		computeLocalTransform();
		const Matrix44f &m= m_localTransform;
		return Vec3f(m[0][0], m[0][1], m[0][2]);
	}

	Vec3f Frame::getUpGlobal() const
	{
		computeFullTransform();
		Matrix44f &m = m_fullTransform;
		return Vec3f(m[1][0], m[1][1], m[1][2]);
	}

	Vec3f Frame::getAtGlobal() const
	{
		computeFullTransform();
		Matrix44f &m = m_fullTransform;
		return Vec3f(m[2][0], m[2][1], m[2][2]);
	}

	Vec3f Frame::getLeftGlobal() const
	{
		computeFullTransform();
		Matrix44f &m = m_fullTransform;
		return Vec3f(m[0][0], m[0][1], m[0][2]);
	}

	//Neonic: octree
	void Frame::updateTree( bool NeedFullUpdate )
	{
		for (math::Frame::ChildrenList::const_iterator it = getChildren().begin(); it != getChildren().end(); it++)
			(*it)->updateTree(NeedFullUpdate);
	};

	//-----------------------------------------------------------------------------------
	void Frame::toStream(io::IWriteStream& wf) const
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
	void Frame::fromStream(io::IReadStream& rf)
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
			PFrame child = new Frame;
			child->fromStream( rf );
			addChild( child );
		}
	}

	std::ostream& operator<<(std::ostream& out, const math::Frame& f)
	{
		return out;
	}

	std::istream& operator>>(std::istream& in, math::Frame& f)
	{
		return in;
	}
}