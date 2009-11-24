#include "precompiled.h"

#include <rgde/math/track_camera.h>
#include <rgde/math/camera.h>

namespace math
{

    path_camera::path_camera(camera_ptr camera)
		: m_position(0)
		, base_camera_controller(camera)
    {
        base::lmsg << "path_camera::path_camera()";
    }

    path_camera_ptr path_camera::create(camera_ptr camera)
    {
        return path_camera_ptr(new path_camera(camera));
    }

    void path_camera::move_to(float position)
    {
        m_position = position;
        apply();
    }

    void path_camera::move_forward(float delta)
    {
        move_to(m_position+delta);
    }

    void path_camera::activate()
    {
        apply();
    }

    void path_camera::apply()
    {
        if (m_position < 0)
            m_position = 0;
        if (m_position > m_path.getTotalTime())
            m_position = m_path.getTotalTime();

        if (m_camera)
        {
            try{
                CTrack::Key key = m_path.getKey(m_position);
                //TODO: possible need to ortho basis vectors
                m_camera->look_at(key.m_eye_pos, key.m_lookat_pt, key.m_up);
                m_camera->activate();
            }
            catch(...){}
        }
    }

}