#include "stdafx.h"

#include "transform.h"



namespace rgde
{
namespace scene
{
	using namespace math;

	frame_ptr frame::create()
	{
		return frame_ptr(new frame());
	}

	mat44f frame::make_transform(const point3f& pos, const quatf& rot, const vec3f& s)
	{
		math::mat44f rotation;
		math::setRot(rotation, rot);		

		math::mat44f translate;
		math::setTrans(translate, pos);

		math::mat44f scale;
		math::setScale(scale, s);

		return translate*rotation*scale;
	}

	//mat44f frame::make_transform(const point3f& pos, const EulerAngleXYZf& rot, const vec3f& s)
	//{
	//	math::quatf quat = math::make<quatf, EulerAngleXYZf>(rot);
	//	return make_transform(pos, quat, s);
	//}

	frame::frame()
		: m_need_recompute(false),
		  core::meta_node<frame>("frame"),
		  m_recompute_global_matrix(true),
		  m_scale(1.0f,1.0f,1.0f)
	{
		//property_owner::addProperty(new property<math::vec3f>(m_scale, "Scale"));
		//property_owner::addProperty(new property<point3f>(m_position, "Position", "Point"));
		//property_owner::addProperty(new property<quatf>(m_rotation, "Rotation", "Quaternion"));
	}

	frame::~frame()
	{
	}

	void frame::findFrames(const std::string& str_template, std::vector<frame_ptr>& container)
	{
		const std::string &frame_name = get_name();

		size_t pos = frame_name.find_first_of("_");
		if(pos != -1)
		{
			size_t nBegin = frame_name.find_first_not_of(" ");
			std::string name = frame_name.substr(nBegin, pos - nBegin);

			if(name == str_template)
				container.push_back(this);
		}

		for (children_list::const_iterator it = get_children().begin(); it != get_children().end(); it++)
			(*it)->findFrames(str_template, container);
	}

	void frame::set_position(const point3f& pos)
	{
		m_position = pos;
		m_need_recompute = true;
	}

	void frame::set_rot(const quatf& quat)
	{
		m_rotation = quat;
		m_need_recompute = true;
	}

	void frame::look_at(const vec3f& eye, const vec3f& look_at, const vec3f& up_vec)
	{	
		m_position = eye;
		const math::vec3f& up = up_vec;
		math::vec3f at = look_at - eye;

		{	
			using namespace math;
			vec3f z = makeNormal<float>(at);
			vec3f x = makeNormal<float>(makeCross(up, z));
			vec3f y = makeCross<float>(z, x);

			Matrix33f mat = makeAxes<Matrix33f>(x, y, z);
			set(m_rotation, mat); 
		}

		m_need_recompute = true;
	}

	void frame::set_scale(const vec3f& s)
	{
		m_scale = s;
		m_need_recompute = true;
	}

	const mat44f & frame::get_local_tm() const
	{
		computeLocalTransform();
		return m_local_tm;
	}

	const mat44f & frame::get_full_tm() const
	{
        computeFullTransform();
		return m_fullTransform;
	}

	void frame::debug_draw() const
	{
		//const float l = 10.5f;
		//math::point3f p = get_world_pos();

		//math::point3f X = p + l * getLeftGlobal();
		//math::point3f Y = p + l * getUpGlobal();
		//math::point3f Z = p + l * getAtGlobal();

		//render::lines3d& line_manager = render::render_device::get().get_lines3d();
		//line_manager.add_line( p, X, math::Red );
		//line_manager.add_line( p, Y, math::Green );
		//line_manager.add_line( p, Z, math::Blue );
	}

	void frame::computeLocalTransform() const
	{
		if (!m_need_recompute)
			return;

		math::mat44f rotation;
		math::setRot(rotation, m_rotation);		

		math::mat44f translate;
		math::setTrans(translate, m_position);
		
		math::mat44f scale;
		math::setScale(scale, m_scale);

		m_local_tm = translate * rotation * scale;

		m_need_recompute = false;
		m_recompute_global_matrix = true;
	}

	void frame::computeFullTransform() const 
	{
		if (m_need_recompute)
			computeLocalTransform();

		if (!m_recompute_global_matrix)
			return;

		computeLocalTransform();

		if (get_parent())
			m_fullTransform = get_parent()->get_full_tm() * m_local_tm;
		else
			m_fullTransform = m_local_tm;

		m_recompute_global_matrix = false;
	}

	void frame::on_parent_change()
	{
		m_need_recompute = true;
	}

	point3f frame::get_world_pos() const 
	{
		computeFullTransform();
		const  mat44f &m	= m_fullTransform;
		return point3f(m.mData[12], m.mData[13], m.mData[14]);
	}

	// left(right)  up          at
	//xaxis.x     yaxis.x     zaxis.x
	//xaxis.y     yaxis.y     zaxis.y
	//xaxis.z     yaxis.z     zaxis.z
	vec3f frame::getUp() const 
	{
		computeLocalTransform();
		const mat44f &m= m_local_tm;
		return vec3f(m[1][0], m[1][1], m[1][2]);
	}
	vec3f frame::getAt() const 
	{
		computeLocalTransform();
		const mat44f &m= m_local_tm;
		return vec3f(m[2][0], m[2][1], m[2][2]);
	}
	vec3f frame::getLeft() const 
	{
		computeLocalTransform();
		const mat44f &m= m_local_tm;
		return vec3f(m[0][0], m[0][1], m[0][2]);
	}

	vec3f frame::getUpGlobal() const
	{
		computeFullTransform();
		mat44f &m = m_fullTransform;
		return vec3f(m[1][0], m[1][1], m[1][2]);
	}

	vec3f frame::getAtGlobal() const
	{
		computeFullTransform();
		mat44f &m = m_fullTransform;
		return vec3f(m[2][0], m[2][1], m[2][2]);
	}

	vec3f frame::getLeftGlobal() const
	{
		computeFullTransform();
		mat44f &m = m_fullTransform;
		return vec3f(m[0][0], m[0][1], m[0][2]);
	}

	void frame::update( bool NeedFullUpdate )
	{
		typedef children_list::const_iterator iter;
		const children_list& list = get_children();

		for (iter it = list.begin(); it != list.end(); it++)
			(*it)->update(NeedFullUpdate);
	};
}
}