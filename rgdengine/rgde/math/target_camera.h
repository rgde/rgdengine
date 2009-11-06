#pragma once

#include <rgde/math/camera_controller.h>

namespace math
{

    typedef boost::shared_ptr<class target_camera> target_camera_ptr;

    // camera controller : "Target Camera"
    class target_camera: public base_camera_controller
    {
        target_camera(camera_ptr camera);

    public:
        static target_camera_ptr create(camera_ptr camera);

        //position
        void set_position(const vec3f& up, const vec3f& eye, const vec3f& look_at);
        void get_pos(vec3f& up, vec3f& eye, vec3f& look_at);

        //movement
        void move_forward(float delta);
        void move_back(float delta) {move_forward(-delta);}

        //rotation
        void rotate_right(float angle);
        void rotate_left(float angle) {rotate_right(-angle);}
        void rotate_up(float angle);
        void rotate_down(float angle) {rotate_up(-angle);}

	    //tilt
        void rotate_cw(float);
        void rotate_ccw(float angle) {rotate_cw(-angle);}

        void activate();

    private:
        void apply();
        void do_ortho_normal();

        vec3f m_up;
        vec3f m_eye_pos;
        vec3f m_lookat_pt;
    };

} //namespace math