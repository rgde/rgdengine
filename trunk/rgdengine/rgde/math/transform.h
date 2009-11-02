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
		static Matrix44f make_transform(const Point3f& pos, const Quatf& rot, const Vec3f& s);
		static Matrix44f make_transform(const Point3f& pos, const EulerAngleXYZf& rot, const Vec3f& s);

							frame();
		virtual				~frame();

		const Point3f&		get_pos()		const {return m_position;}
		Point3f				get_world_pos() const;
		void				set_position(const Point3f& pos);
    
		const Quatf&		get_rot()		const {return m_rotation;}
		void				set_rot(const Quatf& quat);

		void				look_at(const Vec3f& vEyePt, const Vec3f& vLookatPt, const Vec3f& vUpVec);

		/// it must be set directly to shader params
		const Matrix44f&			get_local_tm() const;
		const Matrix44f&			get_full_tm()  const;

		virtual void				debug_draw() const;

		inline const Vec3f&			get_scale()	const { return m_scale;}
		void						set_scale(const Vec3f& s);

		// directional vectors
		Vec3f						getUp()		const;
		Vec3f						getAt()		const;
		Vec3f						getLeft()	const;

		Vec3f						getUpGlobal() const;
		Vec3f						getAtGlobal() const;
		Vec3f						getLeftGlobal() const;

		//Neonic: octree
		virtual void				update( bool NeedFullUpdate =0 );

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