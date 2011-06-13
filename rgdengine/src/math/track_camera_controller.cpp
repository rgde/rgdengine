#include "precompiled.h"

#include <rgde/math/track_camera_controller.h>
#include <rgde/math/camera.h>

namespace math
{
    track_camera_controller::track_camera_controller(camera_ptr camera)
		: m_position(0)
		, base_camera_controller(camera)
    {
        base::lmsg << "track_camera_controller::track_camera_controller()";
    }

    track_camera_controller_ptr track_camera_controller::create(camera_ptr camera)
    {
        return track_camera_controller_ptr(new track_camera_controller(camera));
    }

    void track_camera_controller::move_to(float position)
    {
        m_position = position;
        apply();
    }

    void track_camera_controller::move_forward(float delta)
    {
        move_to(m_position+delta);
    }

    void track_camera_controller::activate()
    {
        apply();
    }

    void track_camera_controller::apply()
    {
        if (m_position < 0)
            m_position = 0;
        if (m_position > m_path.total_time())
            m_position = m_path.total_time();

        if (m_camera)
        {
            try{
                track_t::key_t key = m_path.key(m_position);
                //TODO: possible need to ortho basis vectors
                m_camera->lookat(key.m_eye_pos, key.m_lookat_pt, key.m_up);
                m_camera->activate();
            }
            catch(...){}
        }
    }

}