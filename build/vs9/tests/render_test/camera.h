#pragma once

#include "transform.h"
#include "frustum.h"

namespace rgde
{
namespace scene
{
	class camera : public frame
	{
	public:
		camera();
		~camera();

		virtual void	set_projection(float fovy, float aspect, float z_near, float z_far);
		virtual void	activate();

		const math::mat44f  get_view_matrix()	const;
		const math::mat44f& get_proj_matrix()	const {return m_projection;}

		math::frustum& get_frustum() { return m_frustum; }
		const math::frustum& get_frustum() const { return m_frustum; }

		virtual void	 set_priority(unsigned priority) {}
		virtual unsigned get_priority() const {return 0;}

	protected:
		math::mat44f m_projection;
		math::frustum m_frustum;
	};

	typedef boost::intrusive_ptr<camera> camera_ptr;


	typedef boost::shared_ptr<class base_camera_controller> camera_controller_ptr;

	//camera controller
	class base_camera_controller : boost::noncopyable
	{
	public:
		virtual ~base_camera_controller() {}

		// camera managing
		void bind(camera_ptr cam)				{m_camera = cam;}
		const camera_ptr& get_camera()	const	{return m_camera;}
		virtual void activate() = 0;

	protected:
		base_camera_controller(camera_ptr cam = camera_ptr()) 
			: m_camera(cam){}

	protected:
		camera_ptr m_camera;
	};

	typedef boost::shared_ptr<class free_camera> free_camera_ptr;

	//free camera controller
	class free_camera: public base_camera_controller
	{
		free_camera(camera_ptr camera);

	public:
		static free_camera_ptr create(camera_ptr camera);

		//position
		void set_position(const math::vec3f& up, const math::vec3f& eye, const math::vec3f& look_at);
		void get_pos(math::vec3f& up, math::vec3f& eye, math::vec3f& look_at);

		//moving
		void move_forward(float delta);
		void move_back(float delta) {move_forward(-delta);}
		void move_left(float delta);
		void move_right(float delta) {move_left(-delta);}
		void move_up(float delta);
		void move_down(float delta) {move_up(-delta);}

		//rotation
		void rotate_right(float angle);
		void rotate_left(float angle) {rotate_right(-angle);}
		void rotate_up(float angle);
		void rotate_down(float angle) {rotate_up(-angle);}
		void rotate_cw(float angle);
		void rotate_ccw(float angle) {rotate_cw(-angle);}

		void activate();

	private:
		void apply();
		void do_ortho_normal();

		math::vec3f m_up;
		math::vec3f m_eye_pos;
		math::vec3f m_lookat_pt;
	};
}
}