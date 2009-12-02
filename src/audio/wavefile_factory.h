#pragma once

namespace rgde
{
	namespace audio
	{
		class wave_file;

		// This is a singleton factory that manages .wav file memory.
		// Currently it just loads a wav file when it is requested
		// and keeps it in memory. Eventually you would want it to keep
		// only frequently requested and short duration wav files in memory
		class wave_file_factory
		{
		public:
			wave_file_factory();
			virtual ~wave_file_factory();

			static wave_file_factory* instance();

			wave_file* get(const char*);

		protected:
			static wave_file_factory* sm_instance;

			typedef std::map<std::string, wave_file*> wave_map;
			typedef wave_map::iterator wave_iter;

			wave_map m_wavs;
		};

		inline wave_file_factory* wave_file_factory::instance()
		{
			if (!sm_instance)
				sm_instance = new wave_file_factory();

			return sm_instance;
		}
	}
}