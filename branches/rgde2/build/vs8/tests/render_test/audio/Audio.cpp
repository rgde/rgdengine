#include "stdafx.h"
#include "audio.h"
#include "AudioManager.h"

namespace audio
{
	namespace
	{
		std::map<HWND, AudioManager*> managers;

	}

	system::system(void* hwnd)
		: manager(new AudioManager((HWND)hwnd))
	{
			
	}

	system::~system()
	{
		delete manager;
		manager = 0;
	}

	void system::update(int ms_elapsed)
	{
		manager->Update(ms_elapsed);
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
		return manager->LoadAudioTags(file);
	}

	int system::get_num_tags()
	{
		return manager->GetNumAudioTags();
	}

	const char* system::get_tag_name(size_t tag_index)
	{
		return manager->GetAudioTagName((unsigned int)tag_index);
	}

	void system::set_camera(camera* cam)
	{
		manager->SetListenerCamera(cam);
	}

	void system::play_by_name(const char* tag_name)
	{
		manager->Play(tag_name);
	}

	void system::play(size_t tag_index)
	{
		if (const char* name = manager->GetAudioTagName(tag_index))
			manager->Play(name);
	}
}