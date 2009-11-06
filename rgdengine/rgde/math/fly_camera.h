#pragma once

#include <rgde/math/camera_controller.h>

namespace math
{
    typedef boost::shared_ptr<class free_camera> free_camera_ptr;

    //free camera controller
    class free_camera: public base_camera_controller
    {
        free_camera(camera_ptr camera);

    public:
        static free_camera_ptr create(camera_ptr camera);

        //posotion
        void set_position(const vec3f& up, const vec3f& eye, const vec3f& look_at);
        void get_pos(vec3f& up, vec3f& eye, vec3f& look_at);

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

        vec3f m_up;
        vec3f m_eye_pos;
        vec3f m_lookat_pt;
    };

} //namespace math