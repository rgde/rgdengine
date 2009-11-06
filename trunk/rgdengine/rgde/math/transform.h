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
		static matrix44f make_transform(const point3f& pos, const quatf& rot, const vec3f& s);
		static matrix44f make_transform(const point3f& pos, const EulerAngleXYZf& rot, const vec3f& s);

		static frame_ptr	create();				

		virtual				~frame();

		const point3f&		get_pos()		const {return m_position;}
		point3f				get_world_pos() const;
		void				set_position(const point3f& pos);
    
		const quatf&		get_rot()		const {return m_rotation;}
		void				set_rot(const quatf& quat);

		void				look_at(const vec3f& eye, const vec3f& look_at, const vec3f& up_vec);

		/// it must be set directly to shader params
		const matrix44f&			get_local_tm() const;
		const matrix44f&			get_full_tm()  const;

		virtual void				debug_draw() const;

		inline const vec3f&			get_scale()	const { return m_scale;}
		void						set_scale(const vec3f& s);

		// directional vectors
		vec3f						getUp()		const;
		vec3f						getAt()		const;
		vec3f						getLeft()	const;

		vec3f						getUpGlobal() const;
		vec3f						getAtGlobal() const;
		vec3f						getLeftGlobal() const;

		//Neonic: octree
		virtual void				update( bool NeedFullUpdate =0 );

		//Finds frames with names wich contain substring str_template + "_"
		void findFrames(const std::string& str_template, std::vector<frame_ptr>& container);
		
	protected:
		frame();

		virtual void on_parent_change();
		
		void to_stream(io::write_stream& wf) const;
		void from_stream(io::read_stream& rf);

		bool isNeedRecompute()			const {return m_need_recompute;}
		void computeLocalTransform()	const;
		void computeFullTransform()		const;

	protected:
		vec3f					m_scale;
		point3f					m_position;
		quatf					m_rotation;

		mutable bool			m_need_recompute;
		mutable bool			m_recompute_global_matrix;

		mutable matrix44f		m_local_tm;
		mutable matrix44f		m_fullTransform;
	};
	
	std::ostream& operator<<(std::ostream& out, const math::frame& f);
	std::istream& operator>>(std::istream& in, math::frame& f);
}