#pragma once

#include "xml_node.h"

namespace rgde
{

namespace scene
{
	class frame;
	typedef boost::intrusive_ptr<frame>  frame_ptr;

	class frame :	boost::noncopyable, 
					public core::meta_node<frame>
	{
	public:
		static math::mat44f make_transform(const math::point3f& pos, const math::quatf& rot, const math::vec3f& s);
		//static math::mat44f make_transform(const math::point3f& pos, const EulerAngleXYZf& rot, const vec3f& s);

		static frame_ptr	create();				

		virtual				~frame();

		const math::point3f& get_pos()		const {return m_position;}
		math::point3f		get_world_pos() const;
		void				set_position(const math::point3f& pos);
    
		const math::quatf&		get_rot() const {return m_rotation;}
		void				set_rot(const math::quatf& quat);

		void				look_at(const math::vec3f& eye, const math::vec3f& look_at, const math::vec3f& up_vec);

		/// it must be set directly to shader params
		const math::mat44f&			get_local_tm() const;
		const math::mat44f&			get_full_tm()  const;

		virtual void				debug_draw() const;

		inline const math::vec3f&	get_scale()	const { return m_scale;}
		void						set_scale(const math::vec3f& s);

		// directional vectors
		math::vec3f			getUp()		const;
		math::vec3f			getAt()		const;
		math::vec3f			getLeft()	const;

		math::vec3f			getUpGlobal() const;
		math::vec3f			getAtGlobal() const;
		math::vec3f			getLeftGlobal() const;

		//Neonic: octree
		virtual void				update( bool NeedFullUpdate =0 );

		//Finds frames with names wich contain substring str_template + "_"
		void findFrames(const std::string& str_template, std::vector<frame_ptr>& container);
		
	protected:
		frame();

		virtual void on_parent_change();
		
		bool isNeedRecompute()			const {return m_need_recompute;}
		void computeLocalTransform()	const;
		void computeFullTransform()		const;

	protected:
		math::vec3f				m_scale;
		math::point3f			m_position;
		math::quatf				m_rotation;

		mutable bool			m_need_recompute;
		mutable bool			m_recompute_global_matrix;

		mutable math::mat44f	m_local_tm;
		mutable math::mat44f	m_fullTransform;
	};
}

}