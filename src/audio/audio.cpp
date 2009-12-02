#include "stdafx.h"
#include "audio.h"
#include "audio_manager.h"

namespace rgde
{
	namespace audio
	{

		namespace
		{
			std::map<HWND, audio_manager*> managers;

		}

		system::system(void* hwnd)
			: manager(new audio_manager((HWND)hwnd))
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

		void system::play(const char* tag_name)
		{
			manager->play(tag_name);
		}

		void system::play(size_t tag_index)
		{
			if (const char* name = manager->get_tag_name(tag_index))
				manager->play(name);
		}
	}
}