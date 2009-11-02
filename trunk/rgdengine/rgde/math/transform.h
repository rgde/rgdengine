#pragma once

#include <rgde/io/io.h>
#include <rgde/core/xml_node.h>
#include <rgde/math/types3d.h>

namespace math
{
	class frame;
	typedef boost::intrusive_ptr<frame>  frame_ptr;

	class frame :	public io::serialized_object,
					boost::noncopyable, 
					public core::meta_node<frame>
	{
	public:
		static Matrix44f makeTransformMatrix(const Point3f& pos, const Quatf& rot, const Vec3f& s);
		static Matrix44f makeTransformMatrix(const Point3f& pos, const EulerAngleXYZf& rot, const Vec3f& s);

							frame();
		virtual				~frame();

		const Point3f&		getPosition()		const {return m_position;}
		Point3f				getGlobalPosition() const;
		void				setPosition(const Point3f& pos);
    
		const Quatf&		getRotation()		const {return m_rotation;}
		void				setRotation(const Quatf& quat);

		void				lookAt(const Vec3f& vEyePt, const Vec3f& vLookatPt, const Vec3f& vUpVec);

		/// it must be set directly to shader params
		const Matrix44f&			getLocalTransform() const;
		const Matrix44f&			getFullTransform()  const;

		virtual void				debugDraw() const;

		inline const Vec3f&			getScale()	const { return m_scale;}
		void						setScale(const Vec3f& s);

		// directional vectors
		Vec3f						getUp()		const;
		Vec3f						getAt()		const;
		Vec3f						getLeft()	const;

		Vec3f						getUpGlobal() const;
		Vec3f						getAtGlobal() const;
		Vec3f						getLeftGlobal() const;

		//Neonic: octree
		virtual void				updateTree( bool NeedFullUpdate =0 );

		//Finds frames with names wich contain substring strTemplate + "_"
		void findFrames(const std::string& strTemplate, std::vector<frame_ptr>& container);
		
	protected:
		virtual void on_parent_change();
		
		void toStream(io::write_stream& wf) const;
		void fromStream(io::read_stream& rf);

		bool isNeedRecompute()			const {return m_bIsNeedRecompute;}
		void computeLocalTransform()	const;
		void computeFullTransform()		const;

	protected:
		Vec3f					m_scale;
		Point3f					m_position;
		Quatf					m_rotation;

		mutable bool			m_bIsNeedRecompute;
		mutable bool			m_bNeedRecomputeGlobalMatrix;

		mutable Matrix44f		m_localTransform;
		mutable Matrix44f		m_fullTransform;
	};
	
	std::ostream& operator<<(std::ostream& out, const math::frame& f);
	std::istream& operator>>(std::istream& in, math::frame& f);
}