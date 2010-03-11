#include "stdafx.h"
#include "audio.h"
#include "audio_manager.h"

namespace rgde
{
	namespace audio
	{
		world_object::world_object()
			:	m_pos(0.0f, 0.0f, 0.0f),
			m_vel(0.0f, 0.0f, 0.0f)
		{
		}

		void world_object::get_position(float& x, float& y, float& z) const
		{
			x = m_pos.x;
			y = m_pos.y;
			z = m_pos.z;
		}

		const vec3& world_object::get_position() const
		{
			return m_pos;
		}

		void world_object::set_position(float x, float y, float z)
		{
			m_pos.x = x;
			m_pos.y = y;
			m_pos.z = z;
		}

		const vec3& world_object::get_velocity() const
		{
			return m_vel;
		}

		void world_object::set_velocity(float x, float y, float z)
		{
			m_vel.x = x;
			m_vel.y = y;
			m_vel.z = z;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		system::system(core::windows::handle handle)
			: manager(new audio_manager(handle))
		{
		}

		system::~system()
		{
			delete manager;
			manager = 0;
		}

		void system::update(int ms_elapsed)
		{
			manager->update(ms_elapsed);
		}

		void system::stop_all()
		{
			manager->StopAll();
		}

		void system::set_volume(int volume)
		{
			manager->SetOverallVolume(volume);
		}

		bool system::load(const char* file)
		{
			return manager->load_audio_tags(file);
		}

		bool system::load(xml::node node)
		{
			return manager->load_audio_tags(node);
		}

		int system::get_num_tags()
		{
			return manager->get_num_tags();
		}

		const char* system::get_tag_name(size_t tag_index)
		{
			return manager->get_tag_name((unsigned int)tag_index);
		}

		void system::set_camera(camera* cam)
		{
			manager->set_listener_camera(cam);
		}

		void system::play(const char* tag_name, int ms_duration, int ms_delay,	
				listener_ptr listener, world_object_ptr world_object)
		{
			manager->play(tag_name, 0, ms_duration, ms_delay);//TODO:, listener, world_object);
		}

		void system::play(size_t tag_index, int ms_duration, int ms_delay,	
				listener_ptr listener, world_object_ptr world_object)
		{
			if (const char* name = manager->get_tag_name(tag_index))
				manager->play(name, 0, ms_duration, ms_delay); //TODO: listener, world_object
		}
	}
}